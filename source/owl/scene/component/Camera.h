/**
 * @file Camera.h
 * @author Silmaen
 * @date 23/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../SceneCamera.h"
#include "math/YamlSerializers.h"
#include <magic_enum/magic_enum.hpp>

namespace owl::scene::component {

/**
 * @brief Class Camera.
 */
struct OWL_API Camera {
	/// If camera is the primary one.
	bool primary = true;
	/// Has camera fixed ratio.
	bool fixedAspectRatio = false;
	/// The scene camera.
	SceneCamera camera;
	/**
	 * @brief Get the class title.
	 * @return The class title.
	 */
	static auto name() -> const char* { return "Camera"; }
	/**
	 * @brief Get the YAML key for this component
	 * @return The YAML key.
	 */
	static auto key() -> const char* { return "Camera"; }

	/**
	 * @brief Write this component to a YAML context.
	 * @param ioOut The YAML context.
	 */
	void serialize(YAML::Emitter& ioOut) const {
		ioOut << YAML::Key << key();
		ioOut << YAML::BeginMap;// CameraComponent
		ioOut << YAML::Key << "camera" << YAML::Value;
		ioOut << YAML::BeginMap;// Camera
		ioOut << YAML::Key << "projectionType" << YAML::Value
			  << std::string(magic_enum::enum_name(camera.getProjectionType()));
		ioOut << YAML::Key << "perspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
		ioOut << YAML::Key << "perspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
		ioOut << YAML::Key << "perspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
		ioOut << YAML::Key << "orthographicSize" << YAML::Value << camera.getOrthographicSize();
		ioOut << YAML::Key << "orthographicNear" << YAML::Value << camera.getOrthographicNearClip();
		ioOut << YAML::Key << "orthographicFar" << YAML::Value << camera.getOrthographicFarClip();
		ioOut << YAML::EndMap;// Camera
		ioOut << YAML::Key << "primary" << YAML::Value << primary;
		ioOut << YAML::Key << "fixedAspectRatio" << YAML::Value << fixedAspectRatio;
		ioOut << YAML::EndMap;// Camera
	}

	/**
	 * @brief Read this component from YAML node.
	 * @param iNode The YAML node to read.
	 */
	void deserialize(const YAML::Node& iNode) {
		auto cameraProps = iNode["camera"];
		const auto projType =
				magic_enum::enum_cast<SceneCamera::ProjectionType>(cameraProps["projectionType"].as<std::string>());
		if (projType.has_value())
			camera.setProjectionType(projType.value());
		camera.setPerspectiveVerticalFOV(cameraProps["perspectiveFOV"].as<float>());
		camera.setPerspectiveNearClip(cameraProps["perspectiveNear"].as<float>());
		camera.setPerspectiveFarClip(cameraProps["perspectiveFar"].as<float>());

		camera.setOrthographicSize(cameraProps["orthographicSize"].as<float>());
		camera.setOrthographicNearClip(cameraProps["orthographicNear"].as<float>());
		camera.setOrthographicFarClip(cameraProps["orthographicFar"].as<float>());

		primary = iNode["primary"].as<bool>();
		fixedAspectRatio = iNode["fixedAspectRatio"].as<bool>();
	}
};

}// namespace owl::scene::component
