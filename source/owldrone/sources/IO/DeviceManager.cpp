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
 * @brief List the Camera Devices.
 * @param listToUpdate The device's list to update.
 *
 * @note see: https://learn.microsoft.com/en-us/windows/win32/directshow/using-the-system-device-enumerator
 */
void enumerateCameraDevices(DeviceManager::DeviceList &listToUpdate) {
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
void enumerateSerialDevices(DeviceManager::DeviceList &listToUpdate) {
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

namespace drone::IO {

/**
 * @brief List the Camera Devices.
 * @param listToUpdate The device's list to update.
 */
void enumerateCameraDevices(DeviceManager::DeviceList &listToUpdate) {
	// TODO
}

/**
 * @brief List The serial Port Available
 * @param listToUpdate The device's list to update.
 */
void enumerateSerialDevices(DeviceManager::DeviceList &listToUpdate) {
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
	return std::count_if(devices.begin(), devices.end(), [&type](const Device &T) { return T.type == type; });
}

}// namespace drone::IO