/**
 * @file CameraSystem.cpp
 * @author Silmaen
 * @date 28/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "CameraSystem.h"
#include "DroneSettings.h"
#include "math/simpleFunctions.h"

namespace drone::IO {

CameraSystem::CameraSystem() {
	resize({1, 1});
}

CameraSystem::~CameraSystem() = default;

void CameraSystem::onUpdate(const owl::core::Timestep &ts) {
	++frameCount;
	auto &cameraManager = owl::input::video::Manager::get();
	auto &settings = IO::DroneSettings::get();

	// read image from camera
	if (frameCount % frameCheck == 0) {
		if (const size_t nbCam = cameraManager.getDeviceCount(); nbCam == 0) {
			settings.useCamera = false;
		} else {
			if (settings.cameraId > static_cast<int32_t>(nbCam)) {
				settings.cameraId = 0;
			}
		}
		if (!cameraManager.isOpened()) {
			// open the right one
			if (settings.useCamera) {
				OWL_TRACE("Opening camera {}", settings.cameraId)
				cameraManager.open(static_cast<size_t>(settings.cameraId));
			}
		}
		if (cameraManager.isOpened()) {
			if (!settings.useCamera) {
				cameraManager.close();
			}
			// Define the frameSkip
			if (const float frameRateTs = ts.getStabilizedFps(); frameRateTs > 0) {
				frameSkip = static_cast<int32_t>(240.f / frameRateTs);
			}
		}
	}

	if (frameSkip <= 0 || frameCount % frameSkip == 0) {
		if (cameraManager.isOpened()) {
			cameraManager.fillFrame(frame);
		} else {
			resize({1, 1});
			uint8_t color[] = {45, 87, 96};
			frame->setData(&color, 3);
		}
	}
}

void CameraSystem::resize(const owl::math::FrameSize &_size) {
	if (_size == size)
		return;
	size = _size;
	frame.reset();
	frame = owl::renderer::Texture2D::create(size, false);
}

void CameraSystem::setCamera(int32_t id) {
	auto &settings = DroneSettings::get();
	auto &videoInputManager = owl::input::video::Manager::get();
	id = owl::math::clamp(id, 0, static_cast<int32_t>(videoInputManager.getDeviceCount() - 1));
	settings.cameraId = id;
	if (id == videoInputManager.getCurrentDeviceId())
		return;
	OWL_TRACE("Opening camera {}", id)
	videoInputManager.open(static_cast<size_t>(id));
}

void CameraSystem::actualiseList() {
	auto &videoInputManager = owl::input::video::Manager::get();
	videoInputManager.updateDeviceList();
	if (videoInputManager.getDeviceCount() == 0)
		OWL_WARN("No video capture device found.")
}

std::vector<std::string> CameraSystem::getListOfCameraNames() const {
	return owl::input::video::Manager::get().getDevicesNames();
}
int32_t CameraSystem::getCurrentCameraId() const {
	return owl::input::video::Manager::get().getCurrentDeviceId();
}
std::string CameraSystem::getCurrentCameraName() const {
	auto &sys = owl::input::video::Manager::get();
	if (!sys.isOpened())
		return "";
	return sys.getDevicesNames()[static_cast<size_t>(sys.getCurrentDeviceId())];
}

void CameraSystem::invalidate() {
	size = {0, 0};
	frameSkip = 0;
	frameCheck = 50;
	frameCount = 0;
	frame.reset();
}

}// namespace drone::IO
