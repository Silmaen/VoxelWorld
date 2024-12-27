/**
 * @file PhysicCollider.h
 * @author Silmaen
 * @date 12/26/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "math/vectors.h"

namespace owl::scene::component {

/**
 * @brief Definition of a collider.
 */
struct OWL_API PhysicCollider {
	/// The size of the collider.
	math::vec3f size{1, 1, 1};
	/// The density of the material.
	float density{1.f};
	/// The restitution coefficient in case of contact.
	float restitution{0};
	/// The friction coefficient for contact.
	float friction{0.5f};
};

}// namespace owl::scene::component
