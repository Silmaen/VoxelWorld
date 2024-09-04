/**
 * @file input.cpp
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

Type Input::s_type = Type::GLFW;
uniq<Input> Input::s_instance = nullptr;

Input::~Input() = default;


void Input::init(const Type& iType) {
	if (s_instance)
		s_instance.reset();
	s_type = iType;
	switch (s_type) {
		case Type::GLFW:
			s_instance = mkUniq<glfw::Input>();
			return;
		case Type::Null:
			s_instance = mkUniq<null::Input>();
	}
}

void Input::invalidate() {
	if (s_instance)
		s_instance.reset();
}

auto Input::isKeyPressed(const KeyCode iKeycode) -> bool {
	if (s_instance)
		return s_instance->isKeyPressed_impl(iKeycode);
	return false;
}

auto Input::isMouseButtonPressed(const MouseCode iMouseCode) -> bool {
	if (s_instance)
		return s_instance->isMouseButtonPressed_impl(iMouseCode);
	return false;
}

auto Input::getMouseX() -> float { return getMousePos().x(); }

auto Input::getMouseY() -> float { return getMousePos().y(); }

auto Input::getMousePos() -> math::vec2 {
	if (s_instance)
		return s_instance->getMousePos_impl();
	return {};
}

void Input::injectKey(const KeyCode iKeycode) {
	if (s_instance)
		s_instance->injectKey_impl(iKeycode);
}

void Input::injectMouseButton(const MouseCode iMouseCode) {
	if (s_instance)
		s_instance->injectMouseButton_impl(iMouseCode);
}

void Input::injectMousePos(const math::vec2& iMousePos) {
	if (s_instance)
		s_instance->injectMousePos_impl(iMousePos);
}

void Input::resetInjection() {
	if (s_instance)
		s_instance->resetInjection_impl();
}

}// namespace owl::input
