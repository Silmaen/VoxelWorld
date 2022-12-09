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

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
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
	 * @brief Keyboard pressed check
	 * @param keycode The Key to check
	 * @return True if pressed
	 */
	inline static bool isKeyPressed(KeyCode keycode) {
		return instance->isKeyPressedImpl(keycode);
	}
	/**
	 * @brief Mouse button pressed check
	 * @param mouseCode Mouse button to check
	 * @return True if pressed
	 */
	inline static bool isMouseButtonPressed(MouseCode mouseCode) {
		return instance->isMouseButtonPressedImpl(mouseCode);
	}
	/**
	 * @brief Get mouse X position
	 * @return Mouse X Position
	 */
	inline static float getMouseX() { return instance->getMouseXImpl(); }
	/**
	 * @brief Get mouse Y position
	 * @return Mouse Y Position
	 */
	inline static float getMouseY() { return instance->getMouseYImpl(); }
	/**
	 * @brief Get mouse position
	 * @return Mouse Position
	 */
	inline static glm::vec2 getMousePos() { return instance->getMousePosImpl(); }

protected:
	/**
	 * @brief Implementation	of keyboard pressed check
	 * @param keycode The Key to check
	 * @return True if pressed
	 */
	virtual bool isKeyPressedImpl(KeyCode keycode) = 0;

	/**
	 * @brief Implementation of mouse button pressed check
	 * @param mouseCode Mouse button to check
	 * @return True if pressed
	 */
	virtual bool isMouseButtonPressedImpl(MouseCode mouseCode) = 0;
	/**
	 * @brief Implementation of get mouse X position
	 * @return Mouse X Position
	 */
	virtual float getMouseXImpl() = 0;
	/**
	 * @brief Implementation of get mouse Y position
	 * @return Mouse Y Position
	 */
	virtual float getMouseYImpl() = 0;
	/**
	 * @brief Get mouse position
	 * @return Mouse Position
	 */
	virtual glm::vec2 getMousePosImpl() = 0;

	/// pointer to the input instance
	static Input *instance;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::input
