/**
 * @file CameraSystem.h
 * @author Silmaen
 * @date 19/10/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "DeviceManager.h"

#ifdef OWL_PLATFORM_WINDOWS
#include <windows.h>

namespace drone::IO {

/**
 * @brief List The serial Port Available
 * @param listToUpdate The device's list to update.
 *
 * @note https://stackoverflow.com/questions/2674048/what-is-proper-way-to-detect-all-available-serial-ports-on-windows
 */
static void enumerateSerialDevices(DeviceManager::DeviceList &listToUpdate) {
	constexpr int maxPort = 32;
	TCHAR lpTargetPath[5000];// buffer to store the path of the COMPORTS
	DWORD test;
	for (int i = 0; i < maxPort; i++)// checking ports from COM0 to COM255
	{
		std::wstring ComName = L"COM" + std::to_wstring(i);// converting to COM0, COM1, COM2
		test = QueryDosDevice(reinterpret_cast<LPCSTR>(ComName.c_str()), static_cast<LPSTR>(lpTargetPath), 5000);
		// Test the return value and error if any
		if (test != 0)//QueryDosDevice returns zero if it didn't find an object
			listToUpdate.push_back({.port = "COM" + std::to_string(i)});
		if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		}
	}
}

}// namespace drone::IO
#elif OWL_PLATFORM_LINUX

namespace drone::IO {


/**
 * @brief List The serial Port Available
 * @param listToUpdate The device's list to update.
 */
static void enumerateSerialDevices([[maybe_unused]] DeviceManager::DeviceList &listToUpdate) {
	// TODO
}

}// namespace drone::IO

#else
#error "Unsupported plateform"
#endif

namespace drone::IO {

DeviceManager::DeviceManager() = default;

DeviceManager::~DeviceManager() = default;

void DeviceManager::updateList() {
	devices.clear();
	// list COM Ports
	enumerateSerialDevices(devices);
	setCurrentDevice(currentDevice->port);
}

owl::shared<Device> DeviceManager::getDeviceByName(const std::string &name) const {
	auto it = std::find_if(devices.begin(), devices.end(), [&name](const Device &T) { return T.name == name; });
	if (it == devices.end()) return nullptr;
	return owl::mk_shared<Device>(*it);
}

owl::shared<Device> DeviceManager::getDeviceByPort(const std::string &port) const {
	auto it = std::find_if(devices.begin(), devices.end(), [&port](const Device &T) { return T.port == port; });
	if (it == devices.end()) return nullptr;
	return owl::mk_shared<Device>(*it);
}

void DeviceManager::setCurrentDevice(const std::string &port) {
	currentDevice = getDeviceByPort(port);
}


}// namespace drone::IO
