/**
 * @file Tag.h
 * @author Silmaen
 * @date 23/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"
#include "fonts/Font.h"

namespace owl::scene::component {

/**
 * @brief A tag component.
 */
struct OWL_API Text {
	/// The text.
	std::string text;
	/// The tex's font.
	shared<fonts::Font> font = nullptr;
	/// The display color.
	math::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
	/// The kerning.
	float kerning = 0.0f;
	/// The line spacing.
	float lineSpacing = 0.0f;
};

}// namespace owl::scene::component
