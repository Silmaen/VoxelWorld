/**
 * @file Window.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Window.h"
#include "glfw/Window.h"
#include "null/Window.h"

namespace owl::input {

uniq<Window> Window::create(const Properties &props) {
	switch (props.winType) {
		case Type::GLFW:
			return mk_uniq<glfw::Window>(props);
		case Type::Null:
			return mk_uniq<null::Window>(props);
	}
	return nullptr;
}

Window::~Window() = default;

}// namespace owl::input
