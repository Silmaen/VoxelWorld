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
	bool primary = true;
	bool fixedAspectRatio = false;
	SceneCamera camera;
};

}// namespace owl::renderer::component
