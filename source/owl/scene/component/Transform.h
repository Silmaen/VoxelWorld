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
#include <glm/gtc/matrix_transform.hpp>

namespace owl::scene::component {

struct OWL_API Transform {

	glm::vec3 translation = {0.f, 0.f, 0.f};
	glm::vec3 rotation = {0.f, 0.f, 0.f};
	glm::vec3 scale = {1.f, 1.f, 1.f};

	Transform() = default;
	explicit Transform(const glm::vec3 &translation_)
		: translation(translation_) {}
	[[nodiscard]] glm::mat4 getTransform() const {
		glm::mat4 rot = glm::rotate(glm::mat4(1.0), rotation.x, {1, 0, 0}) *
						glm::rotate(glm::mat4(1.0), rotation.y, {0, 1, 0}) *
						glm::rotate(glm::mat4(1.0), rotation.z, {0, 0, 1});
		return glm::translate(glm::mat4(1.0f), translation) * rot *
			   glm::scale(glm::mat4(1.f), scale);
	}
};

}// namespace owl::scene::component
