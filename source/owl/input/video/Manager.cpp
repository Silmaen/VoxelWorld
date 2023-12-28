/**
 * @file Manager.cpp
 * @author Silmaen
 * @date 03/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Manager.h"

#if defined(OWL_PLATFORM_WINDOWS)
#include "windows/Device.h"
#elif defined(OWL_PLATFORM_LINUX)
#include "linux/Device.h"
#endif

namespace owl::input::video {

Manager::Manager() {
	updateDeviceList();
}

Manager::~Manager() {
	close();
}

void Manager::updateDeviceList(const bool reset) {
	if (reset)
		devices.clear();
#if defined(OWL_PLATFORM_WINDOWS)
	windows::updateList(devices);
#elif defined(OWL_PLATFORM_LINUX)
	linux64::updateList(devices);
#else
#error "Unsupported platform"
#endif
}

std::vector<std::string> Manager::getDevicesNames() const {
	std::vector<std::string> res;
	res.reserve(devices.size());
	for (const auto &dev: devices)
		res.emplace_back(dev->getName());
	return res;
}
shared<Device> Manager::getCurrentDevice() const {
	if (currentDevice >= maxDevices)
		return nullptr;
	return devices[currentDevice];
}

void Manager::open(size_t id) {
	if (id >= devices.size() || id == currentDevice)
		return;
	devices[id]->open();
	if (devices[id]->isOpened())
		currentDevice = id;
}

void Manager::close() const {
	if (!isOpened())
		return;
	getCurrentDevice()->close();
}

void Manager::fillFrame(shared<renderer::Texture> &frame) const {
	if (!isOpened())
		return;
	getCurrentDevice()->fillFrame(frame);
}

int8_t Manager::getCurrentDeviceId() const {
	if (!isOpened())
		return -1;
	return static_cast<int8_t>(currentDevice);
}


}// namespace owl::input::video
