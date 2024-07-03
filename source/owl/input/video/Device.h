/**
 * @file Device.h
 * @author Silmaen
 * @date 03/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/Core.h"
#include "renderer/Texture.h"

namespace owl::input::video {

/**
 * @brief Class Device.
 */
class OWL_API Device {
public:
	/**
	 * @brief Default Constructor.
	 */
	explicit Device(std::string iName);
	Device(const Device &) = delete;
	Device(Device &&) = delete;
	Device &operator=(const Device &) = delete;
	Device &operator=(Device &&) = delete;

	/**
	 * @brief Destructor.
	 */
	virtual ~Device();

	/**
	 * @brief Get the name of the device.
	 * @return The name of the device.
	 */
	[[nodiscard]] const std::string &getName() const { return m_name; }

	/**
	 * @brief Open this device.
	 */
	virtual void open() = 0;

	/**
	 * @brief Close this device.
	 */
	virtual void close() = 0;

	/**
	 * @brief Check if the device is open.
	 * @return True if open.
	 */
	[[nodiscard]] virtual bool isOpened() const = 0;

	/**
	 * @brief Check the validity of the device.
	 * @return True if valid.
	 */
	[[nodiscard]] virtual bool isValid() const = 0;

	/**
	 * @brief Retrieve a frame.
	 * @param[in] iFrame The frame to update.
	 */
	virtual void fillFrame(shared<renderer::Texture> &iFrame) = 0;

	/**
	 * @brief Get the unique bus information.
	 * @return Bus information.
	 *
	 * @note This bus information identifies the device as unique.
	 */
	[[nodiscard]] const std::string &getBusInfo() const { return m_busInfo; }

	/**
	 * @brief list of PixelFormat
	 */
	enum struct PixelFormat : uint8_t {
		Rgb24,///< R8G8B8 format
		Nv12,///< NV12 pixel format.
		YuYv,///< YUYV pixel format.
		MJpeg,///< MJPG pixel format.
		Unknwon,///< unknown or unsupported pixel format.
	};

	/**
	 * @brief Get the pixel format of the device.
	 * @return The pixel format for this device.
	 */
	[[nodiscard]] const PixelFormat &getPixelFormat() const { return m_pixFormat; }

	/**
	 * @brief Check the support for the pixel format.
	 * @param[in] iPixFormat The pixel format to test.
	 * @return True if supported.
	 */
	[[nodiscard]] static bool isPixelFormatSupported(const PixelFormat &iPixFormat);

protected:
	// NOLINTBEGIN(readability-redundant-member-init)
	/// Bus info of the device, this is unique.
	std::string m_busInfo{};
	/// Name of the device.
	std::string m_name;
	/// The pixel format.
	PixelFormat m_pixFormat = PixelFormat::Unknwon;
	/// The size of the frame.
	math::vec2ui m_size;
	// NOLINTEND(readability-redundant-member-init)

	/**
	 * @brief Convert a raw buffer of pixel to RGB24 format.
	 * @param[in] iInputBuffer The input buffer.
	 * @param[in] iBufferSize The size of the buffer
	 * @return A converted RGB24 buffer.
	 */
	[[nodiscard]] std::vector<uint8_t> getRgbBuffer(const uint8_t *iInputBuffer, int32_t iBufferSize) const;
};

}// namespace owl::input::video
