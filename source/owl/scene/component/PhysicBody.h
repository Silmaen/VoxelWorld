/**
 * @file PhysicBody.h
 * @author Silmaen
 * @date 12/26/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../SceneBody.h"
#include "math/YamlSerializers.h"

namespace owl::scene::component {

/**
 * @brief Physical body information.
 */
struct OWL_API PhysicBody {
	/// The physical body.
	SceneBody body;
	/**
	 * @brief Get the class title.
	 * @return The class title.
	 */
	static auto name() -> const char* { return "Physical body"; }
	/**
	 * @brief Get the YAML key for this component
	 * @return The YAML key.
	 */
	static auto key() -> const char* { return "PhysicBody"; }

	/**
	 * @brief Write this component to a YAML context.
	 * @param ioOut The YAML context.
	 */
	void serialize(YAML::Emitter& ioOut) const {
		ioOut << YAML::Key << key();
		ioOut << YAML::BeginMap;
		ioOut << YAML::Key << "type" << YAML::Value << std::string(magic_enum::enum_name(body.type));
		ioOut << YAML::Key << "fixedRotation" << YAML::Value << body.fixedRotation;
		ioOut << YAML::Key << "colliderSize" << YAML::Value << body.colliderSize;
		ioOut << YAML::Key << "density" << YAML::Value << body.density;
		ioOut << YAML::Key << "restitution" << YAML::Value << body.restitution;
		ioOut << YAML::Key << "friction" << YAML::Value << body.friction;
		ioOut << YAML::EndMap;
	}

	/**
	 * @brief Read this component from YAML node.
	 * @param iNode The YAML node to read.
	 */
	void deserialize(const YAML::Node& iNode) {
		body.type = magic_enum::enum_cast<SceneBody::BodyType>(iNode["type"].as<std::string>())
							.value_or(SceneBody::BodyType::Static);
		body.fixedRotation = iNode["fixedRotation"].as<bool>();
		body.bodyId = 0;
		body.colliderSize = iNode["colliderSize"].as<math::vec3f>();
		body.density = iNode["density"].as<float>();
		body.restitution = iNode["restitution"].as<float>();
		body.friction = iNode["friction"].as<float>();
	}
};

}// namespace owl::scene::component
