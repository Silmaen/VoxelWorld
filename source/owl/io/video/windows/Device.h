/**
 * @file Device.h
 * @author Silmaen
 * @date 03/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#ifdef OWL_PLATFORM_WINDOWS

#include "../Device.h"
#include "WPointer.h"
#include <mfidl.h>
#include <mfreadwrite.h>

/**
 * @brief Windows video devices.
 */
namespace owl::io::video::windows {

/**
 * @brief Search for devices and them to the given list.
 * @param[in,out] ioList The device list to update.
 */
void updateList(std::vector<shared<Device>> &ioList);

/**
 * @brief Class Device.
 */
class Device final : public video::Device {
public:
    /**
     * @brief Default Constructor.
     * @param[in] iMfa Poiter to windows device.
     */
    explicit Device(WPointer<IMFActivate> &iMfa);
    /**
     * @brief Destructor.
     */
    ~Device() override;
    Device(const Device &) = delete;
    Device(Device &&) = delete;
    Device &operator=(const Device &) = delete;
    Device &operator=(Device &&) = delete;

    /**
     * @brief Open this device.
     */
    void open() override;

    /**
     * @brief Close this device.
     */
    void close() override;

    /**
     * @brief Check if the device is open.
     * @return True if open.
     */
    [[nodiscard]] bool isOpened() const override;

    /**
     * @brief Retrieve a frame.
     * @param[in] iFrame The frame to update.
     */
    void fillFrame(shared<renderer::Texture> &iFrame) override;

    /**
     * @brief Check the validity of the device.
     * @return True if valid.
     */
    [[nodiscard]] bool isValid() const override;

private:
    /// Pointer to a media source
    WPointer<IMFMediaSource> m_mediaSource;
    /// Pointer to a media source
    WPointer<IMFActivate> m_devActive;
    /// Pointer to the source reader.
    WPointer<IMFSourceReader> m_sourceReader;
};

}// namespace owl::io::video::windows

#endif
