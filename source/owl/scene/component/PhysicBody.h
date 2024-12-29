/**
 * @file PhysicBody.h
 * @author Silmaen
 * @date 12/26/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

namespace owl::scene::component {

/**
 * @brief Physical body information.
 */
struct OWL_API PhysicBody {
	/// The type of physical body
	enum struct BodyType : uint8_t {
		Static,///< Static but affected by physic.
		Dynamic,///< Position dynamically computed.
		Kinematic///< Kinematic position.
	};
	/// The type of body.
	BodyType type = BodyType::Dynamic;
	/// If the body can rotate.
	bool fixedRotation = false;
	/// Internal body index.
	uint64_t bodyId = 0;
};

}// namespace owl::scene::component
