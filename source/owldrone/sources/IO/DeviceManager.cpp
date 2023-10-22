/**
 * @file CameraSystem.h
 * @author Silmaen
 * @date 19/10/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "DeviceManager.h"

constexpr int maxCamera = 10;

#ifdef OWL_PLATFORM_WINDOWS
#include <dshow.h>
#include <windows.h>

namespace drone::IO {
/**
 * @brief List the Camera Devices.
 * @param listToUpdate The device's list to update.
 *
 * @note see: https://learn.microsoft.com/en-us/windows/win32/directshow/using-the-system-device-enumerator
 */
static void enumerateCameraDevices(DeviceManager::DeviceList &listToUpdate) {
	int32_t id = 0;
	HRESULT hr;
	ICreateDevEnum *pSysDevEnum = nullptr;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC_SERVER,
						  IID_ICreateDevEnum, (void **) &pSysDevEnum);
	if (FAILED(hr))
		return;
	// Obtain a class enumerator for the video compressor category.
	IEnumMoniker *pEnumCat = nullptr;
	hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoCompressorCategory, &pEnumCat, 0);
	if (hr == S_OK) {
		// Enumerate the monikers.
		IMoniker *pMoniker = nullptr;
		ULONG cFetched;
		while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) {
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
										 (void **) &pPropBag);
			if (SUCCEEDED(hr)) {
				// To retrieve the filter's friendly name, do the following:
				VARIANT varName;
				VariantInit(&varName);
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);
				if (SUCCEEDED(hr)) {
					// Display the name in your UI somehow.
					listToUpdate.push_back({.type = Device::DeviceType::Camera, .id = id, .name = varName.pcVal});
					++id;
				}
				VariantClear(&varName);

				// To create an instance of the filter, do the following:
				IBaseFilter *pFilter;
				hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,
											(void **) &pFilter);
				// Now add the filter to the graph.
				//Remember to release pFilter later.
				pPropBag->Release();
			}
			pMoniker->Release();
		}
		pEnumCat->Release();
	}
	pSysDevEnum->Release();
}

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
		test = QueryDosDevice(reinterpret_cast<LPCSTR>(ComName.c_str()), (LPSTR) lpTargetPath, 5000);
		// Test the return value and error if any
		if (test != 0)//QueryDosDevice returns zero if it didn't find an object
			listToUpdate.push_back({.port = "COM" + std::to_string(i), .type = Device::DeviceType::FlightController});
		if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		}
	}
}

}// namespace drone::IO
#elif OWL_PLATFORM_LINUX

#include <fcntl.h>
//#include <linux/v4l2-common.h>
//#include <linux/v4l2-controls.h>
#include <linux/media.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>

namespace drone::IO {

/**
 * @brief List the Camera Devices.
 * @param listToUpdate The device's list to update.
 */
static void enumerateCameraDevices([[maybe_unused]] DeviceManager::DeviceList &listToUpdate) {
	for (int iCam = 0; iCam < maxCamera; ++iCam) {
		std::string test{fmt::format("/dev/video{}", iCam)};
		if (!exists(std::filesystem::path(test)))
			break;
		const int fd = open(test.c_str(), O_RDONLY);
		if (!fd)
			continue;
		v4l2_capability capability{};
		int vcap = ioctl(fd, VIDIOC_QUERYCAP, &capability);
		if (vcap < 0) {
			close(fd);
			continue;
		}
		std::string busInfo;
		std::string card;
		std::string driver;
		if (vcap == 0) {
			OWL_TRACE("First Method {} {} {}.", capability.capabilities, capability.reserved[0], capability.version)
			card = reinterpret_cast<const char *>(capability.card);
			busInfo = reinterpret_cast<const char *>(capability.bus_info);
			driver = reinterpret_cast<const char *>(capability.driver);
		} else {
			OWL_TRACE("Second Method.")
			// second method
			struct media_device_info mdi {};
			int mdiRes = ioctl(fd, MEDIA_IOC_DEVICE_INFO, &mdi);
			if (!mdiRes) {
				driver = mdi.driver;
				if (mdi.bus_info[0])
					busInfo = mdi.bus_info;
				else
					busInfo = fmt::format("platform: {}", driver);
				if (mdi.model[0])
					card = mdi.model;
				else
					card = driver;
			}
		}
		close(fd);
		if (busInfo.empty())
			continue;
		if (std::find_if(listToUpdate.begin(), listToUpdate.end(),
						 [&busInfo](const Device &dev) { return busInfo == dev.busInfo; }) != listToUpdate.end())
			continue;
		OWL_TRACE("{} Found: ({}) [{}] '{}'", iCam, card.c_str(), busInfo.c_str(), driver.c_str())
		listToUpdate.push_back(Device{
				.port = std::move(test),
				.type = Device::DeviceType::Camera,
				.id = iCam,
				.name = std::move(card),
				.busInfo = std::move(busInfo)});
	}
}

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
	// List camera
	enumerateCameraDevices(devices);
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
