/**
 * @file Transform.h
 * @author Silmaen
 * @date 23/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/Core.h"
#include <glm/glm.hpp>

namespace owl::scene::component {

struct OWL_API Transform {
	glm::mat4 transform{1.0f};
	Transform() = default;
	Transform(const glm::mat4 &transform_)
		: transform(transform_) {}
	operator glm::mat4 &() { return transform; }
	operator const glm::mat4 &() const { return transform; }
};

}
