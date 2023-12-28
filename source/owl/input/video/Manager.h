/**
 * @file Manager.h
 * @author Silmaen
 * @date 03/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Device.h"

/**
 * @brief namespace for video input devices.
 */
namespace owl::input::video {

constexpr size_t maxDevices = 10;

/**
 * @brief Class Manager.
 */
class OWL_API Manager {
public:
	Manager(const Manager &) = delete;
	Manager(Manager &&) = delete;
	Manager &operator=(const Manager &) = delete;
	Manager &operator=(Manager &&) = delete;

	/**
	 * @brief Destructor.
	 */
	virtual ~Manager();

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
	[[nodiscard]] size_t getDeviceCount() const { return devices.size(); }

	/**
	 * @brief Request a refresh of the devices list.
	 * @param reset the list will be reset before search, closing all the devices.
	 */
	void updateDeviceList(bool reset = false);

	/**
	 * @brief Get an ordered list of device names.
	 * @return List of device name.
	 */
	[[nodiscard]] std::vector<std::string> getDevicesNames() const;

	/**
	 * @brief Access to the current device.
	 * @return Pointer to the current device of nullptr if not opened.
	 */
	[[nodiscard]] shared<Device> getCurrentDevice() const;
	/**
	 * @brief Access to the current device id.
	 * @return Id of the current device of -1 if not opened.
	 */
	[[nodiscard]] int8_t getCurrentDeviceId() const;

	/**
	 * @brief Check if a device is open.
	 * @return True if a device is open.
	 */
	[[nodiscard]] bool isOpened() const { return currentDevice < maxDevices; }

	/**
	 * @brief Check if some devices exist in the list.
	 * @return True if a device exist.
	 */
	[[nodiscard]] bool hasDevice() const { return !devices.empty(); }

	/**
	 * @brief Open the device with the selected id.
	 * @param id The id in the list of devices.
	 */
	void open(size_t id);

	/**
	 * @brief Close the current device.
	 */
	void close() const;

	/**
	 * @brief Grab a frame and put it into the Texture.
	 * @param frame The texture to update.
	 */
	void fillFrame(shared<renderer::Texture> &frame) const;

private:
	/**
	 * @brief Default Constructor.
	 */
	Manager();
	/// List of devices.
	std::vector<shared<Device>> devices;
	/// index of the current device in the list.
	size_t currentDevice = maxDevices;
};

}// namespace owl::input::video
