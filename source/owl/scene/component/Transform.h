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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace owl::scene::component {

/**
 * @brief The transformation component.
 */
struct OWL_API Transform {
	/// The translation.
	glm::vec3 translation = {0.f, 0.f, 0.f};
	/// The rotation.
	glm::vec3 rotation = {0.f, 0.f, 0.f};
	/// The scale.
	glm::vec3 scale = {1.f, 1.f, 1.f};

	/**
	 * @brief Get the transformation matrix based on TRS.
	 * @return The transformation matrix.
	 */
	[[nodiscard]] glm::mat4 getTransform() const {
		return glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(glm::quat(rotation)) *
			   glm::scale(glm::mat4(1.f), scale);
	}
};

}// namespace owl::scene::component
