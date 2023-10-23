/**
 * @file CameraSystem.cpp
 * @author Silmaen
 * @date 28/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "CameraSystem.h"
#include "DroneSettings.h"
#include <opencv2/opencv.hpp>

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
static void enumerateCameraDevices(CameraSystem::CameraList &listToUpdate) {
	int32_t id = 0;
	HRESULT hr;
	ICreateDevEnum *pSysDevEnum = nullptr;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC_SERVER,
						  IID_ICreateDevEnum, reinterpret_cast<void **>(&pSysDevEnum));
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
			hr = pMoniker->BindToStorage(nullptr, nullptr, IID_IPropertyBag,
										 reinterpret_cast<void **>(&pPropBag));
			if (SUCCEEDED(hr)) {
				VARIANT varName;
				VariantInit(&varName);
				hr = pPropBag->Read(L"FriendlyName", &varName, nullptr);
				if (SUCCEEDED(hr)) {
					listToUpdate.push_back({.id = id, .name = varName.pcVal});
					++id;
				}
				VariantClear(&varName);
				pPropBag->Release();
			}
			pMoniker->Release();
		}
		pEnumCat->Release();
	}
	pSysDevEnum->Release();
}

}// namespace drone::IO
#elif OWL_PLATFORM_LINUX

constexpr int maxCamera = 10;

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
static void enumerateCameraDevices(CameraSystem::CameraList &listToUpdate) {
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
				.id = iCam,
				.name = std::move(card),
				.busInfo = std::move(busInfo)});
	}
}

}// namespace drone::IO

#else
#error "Unsupported plateform"
#endif


namespace drone::IO {

static cv::VideoCapture cama;

CameraSystem::CameraSystem() {
	resize(1, 1);
}

CameraSystem::~CameraSystem() = default;

void CameraSystem::onUpdate(const owl::core::Timestep &ts) {
	++frameCount;
	auto &settings = IO::DroneSettings::get();

	// read image from camera
	if (frameCount % frameCheck == 0) {
		size_t nbCam = getNbCamera();
		if (nbCam == 0) {
			settings.useCamera = false;
		} else {
			if (settings.cameraId > static_cast<int32_t>(nbCam)) {
				settings.cameraId = 0;
			}
		}
		if (!cama.isOpened()) {
			// open the right one
			if (settings.useCamera) {
				cama.open(settings.cameraId);
			}
		}
		if (cama.isOpened()) {
			if (!settings.useCamera) {
				cama.release();
			}
			// Define the frameSkip
			float frameRateTs = ts.getStabilizedFps();
			if (frameRateTs > 0) {
				frameSkip = static_cast<int32_t>(240.f / frameRateTs);
			}
		}
	}

	if (frameSkip <= 0 || frameCount % frameSkip == 0) {
		if (cama.isOpened()) {
			cv::Mat mframe;
			cama >> mframe;
			if (!mframe.empty()) {
				cv::cvtColor(mframe, mframe, cv::COLOR_BGR2RGB);
				resize(static_cast<uint32_t>(mframe.cols), static_cast<uint32_t>(mframe.rows));
				frame->setData(mframe.ptr(), static_cast<uint32_t>(mframe.total() * mframe.elemSize()));
			}
		} else {
			resize(static_cast<uint32_t>(1), static_cast<uint32_t>(1));
			uint8_t color[] = {45, 87, 96};
			frame->setData(&color, static_cast<uint32_t>(3));
		}
	}
}

void CameraSystem::resize(uint32_t nw, uint32_t nh) {
	if (nw == width && nh == height)
		return;
	width = nw;
	height = nh;
	frame.reset();
	frame = owl::renderer::Texture2D::create(width, height, false);
}

void CameraSystem::setCamera(int32_t id) {
	auto &settings = IO::DroneSettings::get();
	int32_t last = settings.cameraId;
	if (id > static_cast<int32_t>(getNbCamera()) || id < 0)
		settings.cameraId = 0;
	else
		settings.cameraId = id;
	if (settings.cameraId != last) {
		if (cama.isOpened()) {
			cama.release();
		}
		cama.open(settings.cameraId);
	}
}
size_t CameraSystem::getNbCamera(bool recompute) {
	if (recompute)
		actualiseList();
	return cameraList.size();
}
int32_t CameraSystem::getCurrentCamera() const {
	return IO::DroneSettings::get().cameraId;
}
void CameraSystem::actualiseList() {
	cameraList.clear();
	enumerateCameraDevices(cameraList);
}

}// namespace drone::IO
