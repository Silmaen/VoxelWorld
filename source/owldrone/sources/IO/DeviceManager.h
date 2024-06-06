/**
 * @file CameraSystem.h
 * @author Silmaen
 * @date 19/10/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <owl.h>

namespace drone::IO {

/**
 * @brief Class DeviceManager.
 */
class DeviceManager {
public:
    /**
     * @brief Destructor.
     */
    virtual ~DeviceManager();

    DeviceManager(const DeviceManager &) = delete;
    DeviceManager(DeviceManager &&) = delete;
    DeviceManager &operator=(const DeviceManager &) = delete;
    DeviceManager &operator=(DeviceManager &&) = delete;

    /**
     * @brief Singleton accessor.
     * @return Instance of this object.
     */
    static DeviceManager &get() {
        static DeviceManager instance;
        return instance;
    }

    bool isMavSdkDevice(const owl::io::serial::Device &iDevice);

private:
    /**
     * @brief Constructor.
     */
    DeviceManager();
};

}// namespace drone::IO
