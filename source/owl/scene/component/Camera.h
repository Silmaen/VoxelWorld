/**
 * @file Camera.h
 * @author Silmaen
 * @date 23/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../SceneCamera.h"

namespace owl::scene::component {

/**
 * @brief Class Camera
 */
struct OWL_API Camera {
public:
	Camera(const Camera &) = default;
	Camera(Camera &&) = default;
	Camera() = default;
	bool primary = true;
	bool fixedAspectRatio = false;
	SceneCamera camera;
private:
};

}// namespace owl::renderer::component
