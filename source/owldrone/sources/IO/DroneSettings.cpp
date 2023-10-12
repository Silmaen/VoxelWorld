/**
 * @file CameraSystem.h
 * @author Silmaen
 * @date 11/10/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "DroneSettings.h"
#include <core/external/yaml.h>

namespace drone::IO {

DroneSettings::DroneSettings() = default;

DroneSettings::~DroneSettings() = default;

void DroneSettings::readFromFile(const std::filesystem::path &file) {
	YAML::Node data = YAML::LoadFile(file.string());
	auto appConfig = data["DroneConfig"];
	if (appConfig) {
		if (appConfig["useCamera"]) useCamera = appConfig["useCamera"].as<bool>();
		if (appConfig["cameraId"]) cameraId = appConfig["cameraId"].as<int>();
	}
}

void DroneSettings::saveToFile(const std::filesystem::path &file) const {
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "DroneConfig" << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "useCamera" << YAML::Value << useCamera;
	out << YAML::Key << "cameraId" << YAML::Value << cameraId;
	out << YAML::EndMap;
	out << YAML::EndMap;

	std::ofstream fileOut(file);
	fileOut << out.c_str();
	fileOut.close();
}

}// namespace drone::IO
