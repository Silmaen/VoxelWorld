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
 * @brief Description of a device
 */
struct Device {
	/**
	 * @brief The different types of devices.
	 */
	enum struct DeviceType {
		None,            ///< No type.
		Camera,          ///< Camera.
		FlightController,///< Flight Controller.
		Radio            ///< Radio device.
	};

	/// The port of the device.
	std::string port{};
	/// The device's type.
	DeviceType type{DeviceType::None};
	/// Camera Id.
	int32_t id = -1;
	/// Device Name.
	std::string name{};
	/// Bus information.
	std::string busInfo;
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
	 * @param type Type of the devices, None means 'all'.
	 * @return The number of devices.
	 */
	[[nodiscard]] size_t getDeviceCount(const Device::DeviceType &type) const;

private:
	/// List of devices.
	DeviceList devices;

	/**
	 * @brief Constructor.
	 */
	DeviceManager();
};

}// namespace drone::IO
