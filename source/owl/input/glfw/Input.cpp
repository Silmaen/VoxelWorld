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

bool Input::isKeyPressed_impl(KeyCode keycode) {
	auto *windows = static_cast<GLFWwindow *>(core::Application::get().getWindow().getNativeWindow());
	const auto state = glfwGetKey(windows, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::isMouseButtonPressed_impl(MouseCode mouseCode) {
	auto *windows = static_cast<GLFWwindow *>(core::Application::get().getWindow().getNativeWindow());
	const auto state = glfwGetMouseButton(windows, mouseCode);
	return state == GLFW_PRESS;
}

glm::vec2 Input::getMousePos_impl() {
	auto *windows = static_cast<GLFWwindow *>(core::Application::get().getWindow().getNativeWindow());
	double X, Y;
	glfwGetCursorPos(windows, &X, &Y);
	return {X, Y};
}

}// namespace owl::input::glfw
