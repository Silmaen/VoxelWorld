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


namespace owl::input::video::windows {

/**
 * @brief Search for devices and them to the given list.
 * @param list The device list to update.
 */
void updateList(std::vector<shared<owl::input::video::Device>> &list);

/**
 * @brief Class Device.
 */
class Device final : public owl::input::video::Device {
public:
	/**
	 * @brief Default Constructor.
	 */
	explicit Device(WPointer<IMFActivate> &mfa);
	/**
	 * @brief Destructor.
	 */
	~Device() override;

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
	 * @param frame The frame to update.
	 */
	void fillFrame(shared<renderer::Texture> &frame) override;

	/**
	 * @brief Check the validity of the device.
	 * @return True if valid.
	 */
	[[nodiscard]] virtual bool isValid() const override;

private:
	/// Pointer to a media source
	WPointer<IMFMediaSource> mediaSource;
	/// Pointer to a media source
	WPointer<IMFActivate> devActive;
	/// Pointer to the source reader.
	WPointer<IMFSourceReader> sourceReader;
};

}// namespace owl::input::video::windows

#endif
