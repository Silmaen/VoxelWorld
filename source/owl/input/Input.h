/**
 * @file Input.h
 * @author Silmaen
 * @date 06/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "KeyCodes.h"
#include "MouseCode.h"
#include "core/Core.h"
#include <glm/glm.hpp>

namespace owl::input {

/**
 * @brief Class Input
 */
class OWL_API Input {
public:
	Input(const Input &) = delete;
	Input(Input &&) = delete;
	Input &operator=(const Input &) = delete;
	Input &operator=(Input &&) = delete;
	/**
	 * @brief Default constructor.
	 */
	Input() = default;
	/**
	 * @brief Destructor.
	 */
	~Input() = default;
	/**
	 * @brief Keyboard pressed check
	 * @param keycode The Key to check
	 * @return True if pressed
	 */
	static bool isKeyPressed(KeyCode keycode);
	/**
	 * @brief Mouse button pressed check
	 * @param mouseCode Mouse button to check
	 * @return True if pressed
	 */
	static bool isMouseButtonPressed(MouseCode mouseCode);
	/**
	 * @brief Get mouse X position
	 * @return Mouse X Position
	 */
	static float getMouseX();
	/**
	 * @brief Get mouse Y position
	 * @return Mouse Y Position
	 */
	static float getMouseY();
	/**
	 * @brief Get mouse position
	 * @return Mouse Position
	 */
	static glm::vec2 getMousePos();
};


}// namespace owl::input
