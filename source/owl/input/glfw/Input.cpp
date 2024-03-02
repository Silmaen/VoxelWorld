/**
 * @file Input.cpp
 * @author Silmaen
 * @date 06/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Input.h"
#include "core/Application.h"
#include "core/external/glfw3.h"

namespace owl::input::glfw {

bool Input::isKeyPressed_impl(const KeyCode iKeycode) {
	auto *windows = static_cast<GLFWwindow *>(core::Application::get().getWindow().getNativeWindow());
	const auto state = glfwGetKey(windows, iKeycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::isMouseButtonPressed_impl(const MouseCode iMouseCode) {
	auto *windows = static_cast<GLFWwindow *>(core::Application::get().getWindow().getNativeWindow());
	const auto state = glfwGetMouseButton(windows, iMouseCode);
	return state == GLFW_PRESS;
}

glm::vec2 Input::getMousePos_impl() {
	auto *windows = static_cast<GLFWwindow *>(core::Application::get().getWindow().getNativeWindow());
	double x, y;
	glfwGetCursorPos(windows, &x, &y);
	return {x, y};
}

}// namespace owl::input::glfw
