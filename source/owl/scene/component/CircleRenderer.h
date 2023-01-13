/**
 * @file CircleRenderer.h
 * @author Silmaen
 * @date 14/01/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <glm/glm.hpp>

namespace owl::scene::component {

/**
 * @brief Struct CircleRenderer
 */
struct OWL_API CircleRenderer {
	glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
	float thickness = 1.0f;
	float fade = 0.005f;
};

}// namespace owl::scene::component
