/**
 * @file CameraSystem.h
 * @author Silmaen
 * @date 19/10/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "DeviceManager.h"

namespace drone::IO {

DeviceManager::DeviceManager() = default;

DeviceManager::~DeviceManager() = default;

bool DeviceManager::isMavSdkDevice([[maybe_unused]] const owl::io::serial::Device &iDevice) { return false; }

}// namespace drone::IO
