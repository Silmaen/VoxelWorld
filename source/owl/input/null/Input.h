/**
 * @file Input.h
 * @author Silmaen
 * @date 02/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../Input.h"
#include <set>

namespace owl::input::null {

/**
 * @brief Class Input
 */
class OWL_API Input : public ::owl::input::Input {
public:
	Input(const Input &) = delete;
	Input(Input &&) = delete;

	/**
	 * @brief Default constructor.
	 */
	Input() = default;

	/**
	 * @brief Destructor.
	 */
	~Input() override = default;

	/**
	 * @brief Keyboard pressed check, private implementation.
	 * @param keycode The Key to check.
	 * @return True if pressed.
	 */
	bool isKeyPressed_impl(KeyCode keycode) override;
	/**
	 * @brief Mouse button pressed check, private implementation.
	 * @param mouseCode Mouse button to check.
	 * @return True if pressed.
	 */
	bool isMouseButtonPressed_impl(MouseCode mouseCode) override;
	/**
	 * @brief Get mouse position, private implementation.
	 * @return Mouse Position.
	 */
	glm::vec2 getMousePos_impl() override;
	/**
	 * @brief Simulate key toggle, private implementation.
	 * @param keycode the key to press/release.
	 */
	void injectKey_impl(KeyCode keycode) override;

	/**
	 * @brief Simulate mouse key toggle, private implementation.
	 * @param mouseCode the key to press/release.
	 */
	void injectMouseButton_impl(MouseCode mouseCode) override;
	/**
	 * @brief Simulate mouse movement, private implementation.
	 * @param mousePos The new mouse pos.
	 */
	void injectMousePos_impl(const glm::vec2 &mousePos) override;

	std::set<KeyCode> keyPressed;
	std::set<MouseCode> mouseBtnPressed;
	glm::vec2 mousePos;
};


}// namespace owl::input::null
