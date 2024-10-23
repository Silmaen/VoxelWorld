/**
 * @file Transform.h
 * @author Silmaen
 * @date 23/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "math/Transform.h"

namespace owl::scene::component {

/**
 * @brief The transformation component.
 */
struct OWL_API Transform {
	/// The transformation.
	math::Transform transform;
};

}// namespace owl::scene::component
