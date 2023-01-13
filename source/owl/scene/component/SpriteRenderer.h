/**
 * @file SpriteRenderer.h
 * @author Silmaen
 * @date 22/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "renderer/Texture.h"

#include <glm/glm.hpp>

namespace owl::scene::component {

/**
 * @brief Struct SpriteRenderer
 */
struct OWL_API SpriteRenderer {
	glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
	shrd<renderer::Texture2D> texture = nullptr;
	float tilingFactor = 1.0f;
};

}// namespace owl::scene::component
