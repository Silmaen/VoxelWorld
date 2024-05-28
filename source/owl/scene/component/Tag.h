/**
 * @file Tag.h
 * @author Silmaen
 * @date 23/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

namespace owl::scene::component {

/**
 * @brief A tag component.
 */
struct OWL_API Tag {
	/// The tag name.
	std::string tag;
} OWL_ALIGN(32);

}// namespace owl::scene::component
