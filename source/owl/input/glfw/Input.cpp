/**
 * @file Input.cpp
 * @author Silmaen
 * @date 06/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "../Input.h"
#include "core/Application.h"
#include "core/external/glfw3.h"


namespace owl::input {

bool Input::isKeyPressed(KeyCode keycode) {
	auto *windows = static_cast<GLFWwindow *>(core::Application::get().getWindow().getNativeWindow());
	auto state = glfwGetKey(windows, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::isMouseButtonPressed(MouseCode mouseCode) {
	auto *windows = static_cast<GLFWwindow *>(core::Application::get().getWindow().getNativeWindow());
	auto state = glfwGetMouseButton(windows, mouseCode);
	return state == GLFW_PRESS;
}

float Input::getMouseX() {
	return getMousePos().x;
}

float Input::getMouseY() {
	return getMousePos().y;
}
glm::vec2 Input::getMousePos() {
	auto *windows = static_cast<GLFWwindow *>(core::Application::get().getWindow().getNativeWindow());
	double X, Y;
	glfwGetCursorPos(windows, &X, &Y);
	return {X, Y};
}

}// namespace owl::input
