/**
 * @file CameraSystem.h
 * @author Silmaen
 * @date 19/10/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "DeviceManager.h"

#ifdef OWL_PLATFORM_WINDOWS
#include <dshow.h>
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
			listToUpdate.push_back({.port = "COM" + std::to_string(i), .type = Device::DeviceType::FlightController});
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
}

size_t DeviceManager::getDeviceCount(const Device::DeviceType &type) const {
	if (type == Device::DeviceType::None)
		return devices.size();
	return static_cast<size_t>(std::count_if(devices.begin(), devices.end(),
											 [&type](const Device &T) {
												 return T.type == type;
											 }));
}

}// namespace drone::IO
