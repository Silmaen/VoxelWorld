/**
 * @file Manager.h
 * @author Silmaen
 * @date 04/06/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Device.h"

namespace owl::io::serial {

/**
 * @brief Class Manager.
 */
class OWL_API Manager final {
public:
    using deviceList = std::vector<Device>;

    Manager(const Manager &) = delete;
    Manager(Manager &&) = delete;
    Manager &operator=(const Manager &) = delete;
    Manager &operator=(Manager &&) = delete;

    /**
     * @brief Destructor.
     */
    ~Manager();

    /**
     * @brief Singleton accessor.
     * @return Instance of the manager.
     */
    static Manager &get() {
        static Manager instance;
        return instance;
    }

    /**
     * @brief Get the number of found devices.
     * @return The number of found devices.
     */
    [[nodiscard]] size_t getDeviceCount() const { return m_devices.size(); }

    /**
     * @brief Request a refresh of the devices list.
     */
    void updateDeviceList();

    /**
     * @brief Check if some devices exist in the list.
     * @return True if a device exist.
     */
    [[nodiscard]] bool hasDevice() const { return !m_devices.empty(); }

private:
    /**
     * @brief Default Constructor.
     */
    Manager();
    /// List of device names.
    deviceList m_devices;
};

}// namespace owl::io::serial
