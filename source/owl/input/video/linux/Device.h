/**
 * @file Device.h
 * @author Silmaen
 * @date 03/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "../Device.h"

#ifdef OWL_PLATFORM_LINUX
#include <linux/videodev2.h>

/**
 * @brief Linux video devices.
 */
namespace owl::input::video::linux64 {

/**
 * @brief Search for devices and them to the given list.
 * @param list The device list to update.
 */
void updateList(std::vector<shared<owl::input::video::Device>> &list);

/**
 * @brief Class Device.
 */
class Device final : public video::Device {
public:
	/**
	 * @brief Consructor using only a file.
	 * @param file The file to read.
	 */
	explicit Device(std::string file);

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
	 * @brief Get the file.
	 * @return File name.
	 */
	[[nodiscard]] const std::string &getFile() const { return file; }

	/**
	 * @brief Retrieve a frame.
	 * @param frame The frame to update.
	 */
	void fillFrame(shared<renderer::Texture> &frame) override;

	/**
	 * @brief Check if this device is valid.
	 * @return True if this device is valid.
	 */
	[[nodiscard]] bool isValid() const override;

private:
	/// The file representation of the device.
	std::string file;
	/// The file handler.
	int fileHandler = 0;
	/// buffer map to the device.
	void *buffer = nullptr;
	/// Buffer info.
	v4l2_buffer bufferInfo{};
	/// if the streaming is started.
	bool streaming = false;

	void printSupportedFormat() const;

	/**
	 * \brief fist suitable pixel format.
	 * \param fd File desciptor, use fileHander if -1. File must already be opened.
	 * \return The first pixel format supported by both device and present code.
	 */
	[[nodiscard]] PixelFormat getFirstSupportedPixelFormat(int32_t fd = -1) const;
};

}// namespace owl::input::video::linux64

#endif
