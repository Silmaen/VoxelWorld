/**
 * @file Input.cpp
 * @author Silmaen
 * @date 02/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Input.h"
#include "core/Application.h"
#include "core/external/glfw3.h"


namespace owl::input::null {

bool Input::isKeyPressed_impl(KeyCode keycode) {
	return keyPressed.contains(keycode);
}

bool Input::isMouseButtonPressed_impl(MouseCode mouseCode) {
	return mouseBtnPressed.contains(mouseCode);
}

glm::vec2 Input::getMousePos_impl() {
	return mousePos;
}

void Input::injectKey_impl(KeyCode keycode) {
	if (keyPressed.contains(keycode)) {
		keyPressed.erase(keycode);
	} else {
		keyPressed.insert(keycode);
	}
}

void Input::injectMouseButton_impl(MouseCode mouseCode) {
	if (mouseBtnPressed.contains(mouseCode)) {
		mouseBtnPressed.erase(mouseCode);
	} else {
		mouseBtnPressed.insert(mouseCode);
	}
}

void Input::injectMousePos_impl(const glm::vec2 &mousePos_) {
	mousePos = mousePos_;
}

}// namespace owl::input::null
