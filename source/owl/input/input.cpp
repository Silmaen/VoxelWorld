/**
 * @file Input.cpp
 * @author Silmaen
 * @date 02/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Input.h"
#include "input/glfw/Input.h"
#include "input/null/Input.h"

namespace owl::input {

Type Input::type = Type::GLFW;
uniq<Input> Input::instance = nullptr;

void Input::init(const Type &type_) {
	if (instance)
		instance.reset();
	type = type_;
	switch (type) {
		case Type::GLFW:
			instance = mk_uniq<glfw::Input>();
			return;
		case Type::Null:
			instance = mk_uniq<null::Input>();
	}
}

void Input::invalidate() {
	if (instance)
		instance.reset();
}

bool Input::isKeyPressed(KeyCode keycode) {
	if (instance)
		return instance->isKeyPressed_impl(keycode);
	return false;
}

bool Input::isMouseButtonPressed(MouseCode mouseCode) {
	if (instance)
		return instance->isMouseButtonPressed_impl(mouseCode);
	return false;
}

float Input::getMouseX() {
	return getMousePos().x;
}

float Input::getMouseY() {
	return getMousePos().y;
}

glm::vec2 Input::getMousePos() {
	if (instance)
		return instance->getMousePos_impl();
	return glm::vec2();
}

void Input::injectKey(KeyCode keycode) {
	if (instance)
		instance->injectKey_impl(keycode);
}

void Input::injectMouseButton(MouseCode mouseCode) {
	if (instance)
		instance->injectMouseButton_impl(mouseCode);
}

void Input::injectMousePos(const glm::vec2 &mousePos) {
	if (instance)
		instance->injectMousePos_impl(mousePos);
}

}// namespace owl::input
