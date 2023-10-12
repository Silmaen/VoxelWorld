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

namespace drone::IO {

static cv::VideoCapture cama;

CameraSystem::CameraSystem() {
	resize(1, 1);
	updateCamList();
}

CameraSystem::~CameraSystem() = default;

void CameraSystem::onUpdate(const owl::core::Timestep &ts) {
	++frameCount;
	auto &settings = IO::DroneSettings::get();

	// read image from camera
	if (frameCount % frameCheck == 0) {
		if (!cama.isOpened()) {
			// open the right one
			if (settings.useCamera) {
				cama.open(settings.cameraId);
				if (!cama.isOpened())// something is wrong, recheck camera list
					updateCamList();
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

void CameraSystem::updateCamList() {
	auto &settings = IO::DroneSettings::get();
	{
		// actualize list
		constexpr uint8_t maxCamList = 10;
		cama.release();
		nbCam = 0;
		for (uint8_t id = 0; id < maxCamList; ++id) {
			cv::VideoCapture cam;
			cam.open(id);
			if (cam.isOpened()) {
				++nbCam;
			} else {
				break;
			}
			cam.release();
		}
	}
	OWL_TRACE("Camera devices actualized. {} found.", nbCam)
	if (settings.cameraId > nbCam)
		settings.cameraId = 0;
}

void CameraSystem::setCamera(int32_t id) {
	auto &settings = IO::DroneSettings::get();
	int32_t last = settings.cameraId;
	if (id > nbCam || id < 0)
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
int32_t CameraSystem::getNbCamera(bool recompute) {
	if (recompute)
		updateCamList();
	return nbCam;
}
int32_t CameraSystem::getCurrentCamera() const {
	return IO::DroneSettings::get().cameraId;
}

}// namespace drone::IO
