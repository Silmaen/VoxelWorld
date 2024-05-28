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
 * @brief Class Camera.
 */
struct OWL_API Camera {
	/// If camera is the primary one.
	bool primary = true;
	/// Has camera fixed ratio.
	bool fixedAspectRatio = false;
	/// The scene camera.
	SceneCamera camera;
} OWL_ALIGN(128);

}// namespace owl::scene::component
