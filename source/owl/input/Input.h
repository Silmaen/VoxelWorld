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
#include "Window.h"
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
	virtual ~Input() = default;

	/**
	 * @brief Initialize the input;
	 * @param type the type of input
	 */
	static void init(const Type &type = Type::GLFW);

	/**
	 * @brief Destroy the inout instance.
	 */
	static void invalidate();

	/**
	 * @brief Keyboard pressed check.
	 * @param keycode The Key to check.
	 * @return True if pressed.
	 */
	static bool isKeyPressed(KeyCode keycode);

	/**
	 * @brief Mouse button pressed check.
	 * @param mouseCode Mouse button to check.
	 * @return True if pressed.
	 */
	static bool isMouseButtonPressed(MouseCode mouseCode);

	/**
	 * @brief Get mouse X position.
	 * @return Mouse X Position.
	 */
	static float getMouseX();

	/**
	 * @brief Get mouse Y position.
	 * @return Mouse Y Position.
	 */
	static float getMouseY();

	/**
	 * @brief Get mouse position.
	 * @return Mouse Position.
	 */
	static glm::vec2 getMousePos();

	/**
	 * @brief Simulate key toggle.
	 * @param keycode the key to press/release.
	 */
	static void injectKey(KeyCode keycode);

	/**
	 * @brief Simulate mouse key toggle.
	 * @param mouseCode the key to press/release.
	 */
	static void injectMouseButton(MouseCode mouseCode);

	/**
	 * @brief Simulate mouse movement.
	 * @param mousePos The new mouse pos.
	 */
	static void injectMousePos(const glm::vec2 &mousePos);

private:
	/// The Input type.
	static Type type;
	/// The input instance.
	static uniq<Input> instance;

	/**
	 * @brief Keyboard pressed check, private implementation.
	 * @param keycode The Key to check.
	 * @return True if pressed.
	 */
	virtual bool isKeyPressed_impl(KeyCode keycode) = 0;
	/**
	 * @brief Mouse button pressed check, private implementation.
	 * @param mouseCode Mouse button to check.
	 * @return True if pressed.
	 */
	virtual bool isMouseButtonPressed_impl(MouseCode mouseCode) = 0;
	/**
	 * @brief Get mouse position, private implementation.
	 * @return Mouse Position.
	 */
	virtual glm::vec2 getMousePos_impl() = 0;
	/**
	 * @brief Simulate key toggle, private implementation.
	 * @param keycode the key to press/release.
	 */
	virtual void injectKey_impl(KeyCode keycode) = 0;

	/**
	 * @brief Simulate mouse key toggle, private implementation.
	 * @param mouseCode the key to press/release.
	 */
	virtual void injectMouseButton_impl(MouseCode mouseCode) = 0;
	/**
	 * @brief Simulate mouse movement, private implementation.
	 * @param mousePos The new mouse pos.
	 */
	virtual void injectMousePos_impl(const glm::vec2 &mousePos) = 0;
};


}// namespace owl::input
