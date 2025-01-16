/**
 * @file Player.h
 * @author Silmaen
 * @date 30/12/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../SceneTrigger.h"
#include "math/YamlSerializers.h"

namespace owl::scene::component {

/**
 * @brief component describing a game trigger.
 */
struct OWL_API Trigger {
	/// The scene trigger.
	SceneTrigger trigger;
	/**
	 * @brief Get the class title.
	 * @return The class title.
	 */
	static auto name() -> const char* { return "Trigger"; }
	/**
	 * @brief Get the YAML key for this component
	 * @return The YAML key.
	 */
	static auto key() -> const char* { return "Trigger"; }

	/**
	 * @brief Write this component to a YAML context.
	 * @param ioOut The YAML context.
	 */
	void serialize([[maybe_unused]] YAML::Emitter& ioOut) const {
		ioOut << YAML::Key << key();
		ioOut << YAML::BeginMap;
		ioOut << YAML::Key << "Type" << YAML::Value << std::string(magic_enum::enum_name(trigger.type));
		ioOut << YAML::EndMap;
	}

	/**
	 * @brief Read this component from YAML node.
	 * @param iNode The YAML node to read.
	 */
	void deserialize([[maybe_unused]] const YAML::Node& iNode) {
		if (iNode["Type"]) {
			const auto triggerType = magic_enum::enum_cast<SceneTrigger::TriggerType>(iNode["Type"].as<std::string>());
			if (triggerType.has_value())
				trigger.type = triggerType.value();
		}
	}
};

}// namespace owl::scene::component
