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

static const std::string s_deviceFriendlyName{"unknown"};
/**
 * @brief Description of a device
 */
struct Device {
	/// The port of the device.
	std::string port{};
	/// Devine Name
	std::string name{};
	/**
	 * @brief Access to a friendly name for the device
	 * @return Friendly name of the device.
	 */
	[[nodiscard]] const std::string &getFriendlyName() const {
		if (name.empty())
			return s_deviceFriendlyName;
		return name;
	}
	/**
	 * @brief Hashing function.
	 * @return Device's hash.
	 */
	[[nodiscard]] size_t hash() const {
		return std::hash<std::string>{}(port) ^ (std::hash<std::string>{}(name) << 1);
	}
};

/**
 * @brief Class DeviceManager.
 */
class DeviceManager {
public:
	using DeviceList = std::vector<Device>;

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

	/**
	 * @brief Update the list of devices.
	 */
	void updateList();

	/**
	 * @brief Get all the devices.
	 * @return List of all devices.
	 */
	[[nodiscard]] const DeviceList &getAllDevices() const { return devices; }

	/**
	 * @brief Get the number of device by type.
	 * @return The number of devices.
	 */
	[[nodiscard]] size_t getDeviceCount() const { return devices.size(); }

	/**
	 * @brief Get device by its name.
	 * @param name Name of device.
	 * @return Pointer to the device of nullptr if not exists.
	 */
	[[nodiscard]] owl::shared<Device> getDeviceByName(const std::string &name) const;

	/**
	* @brief Get device by its port.
	* @param port Port of device.
	* @return Pointer to the device of nullptr if not exists.
	*/
	[[nodiscard]] owl::shared<Device> getDeviceByPort(const std::string &port) const;

	/**
	 * @brief Define the new current device.
	 * @param port The port of the new current device.
	 */
	void setCurrentDevice(const std::string &port);

	/**
	 * @brief Get a pointer to the current Device.
	 * @return Pointer to the current device.
	 */
	[[nodiscard]] const owl::shared<Device> &getCurrentDevice() const { return currentDevice; }

private:
	/// List of devices.
	DeviceList devices;
	/// Pointer to the current device.
	owl::shared<Device> currentDevice = nullptr;
	/**
	 * @brief Constructor.
	 */
	DeviceManager();
};

}// namespace drone::IO
