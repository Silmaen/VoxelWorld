/**
 * @file CameraSystem.h
 * @author Silmaen
 * @date 11/10/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include <owl.h>

namespace drone::IO {

/**
 * @brief Class DroneSettings.
 */
class DroneSettings {
public:
	/**
	 * @brief Destructor.
	 */
	virtual ~DroneSettings();

	DroneSettings(const DroneSettings &) = delete;
	DroneSettings(DroneSettings &&) = delete;
	DroneSettings &operator=(const DroneSettings &) = delete;
	DroneSettings &operator=(DroneSettings &&) = delete;

	/**
	 * @brief Reads the configuration from file.
	 * @param file The file name to read.
	 */
	void readFromFile(const std::filesystem::path &file);

	/**
	 * @brief Save the configuration to file.
	 * @param file The file name to write.
	 */
	void saveToFile(const std::filesystem::path &file) const;

	/**
	 * @brief Access to Settings singleton.
	 * @return Settings instance.
	 */
	static DroneSettings &get() {
		static DroneSettings instance;
		return instance;
	}

	/// If the camera should be used.
	bool useCamera = true;
	/// Id of the last used camera.
	int32_t cameraId = 1;

private:
	/**
	 * @brief Constructor.
	 */
	DroneSettings();
};

}// namespace drone::IO
