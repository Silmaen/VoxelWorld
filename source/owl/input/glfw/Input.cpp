/**
 * @file Input.cpp
 * @author argawaen
 * @date 06/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Input.h"
#include "core/Application.h"
#include <GLFW/glfw3.h>

namespace owl::input {
// Change this to allow more input framework!
Input *Input::instance = new glfw::Input;
}

namespace owl::input::glfw {

Input::Input() = default;

bool Input::isKeyPressedImpl(KeyCode keycode) {
  auto* windows = static_cast<GLFWwindow*>(core::Application::get().getWindow().getNativeWindow());
  auto state = glfwGetKey(windows, keycode);
  return state == GLFW_PRESS ||state == GLFW_REPEAT;
}

bool Input::isMouseButtonPressedImpl(MouseCode mouseCode) {
  auto* windows = static_cast<GLFWwindow*>(core::Application::get().getWindow().getNativeWindow());
  auto state = glfwGetMouseButton(windows, mouseCode);
  return state == GLFW_PRESS;
}

float Input::getMouseXImpl() {
  return getMousePosImpl().x;
}

float Input::getMouseYImpl() {
  return getMousePosImpl().y;
}
glm::vec2 Input::getMousePosImpl() {
  auto* windows = static_cast<GLFWwindow*>(core::Application::get().getWindow().getNativeWindow());
  double X, Y;
  glfwGetCursorPos(windows, &X, &Y);
  return {X, Y};
}

} // namespace owl::input::glfw
