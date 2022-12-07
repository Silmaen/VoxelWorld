/**
 * @file Window.cpp
 * @author damien.lachouette
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Window.h"

namespace owl::window::null {

Window::Window(const window::Properties &) {}

Window::~Window() = default;

void Window::onUpdate() {}

void Window::setVSync(bool) {}

bool Window::isVSync() const { return false; };

} // namespace owl::window::null
