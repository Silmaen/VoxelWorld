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

window::Type Input::ms_type = window::Type::GLFW;
uniq<Input> Input::mus_instance = nullptr;

Input::~Input() = default;


void Input::init(const window::Type &iType) {
    if (mus_instance)
        mus_instance.reset();
    ms_type = iType;
    switch (ms_type) {
        case window::Type::GLFW:
            mus_instance = mkUniq<glfw::Input>();
            return;
        case window::Type::Null:
            mus_instance = mkUniq<null::Input>();
    }
}

void Input::invalidate() {
    if (mus_instance)
        mus_instance.reset();
}

bool Input::isKeyPressed(const KeyCode iKeycode) {
    if (mus_instance)
        return mus_instance->isKeyPressed_impl(iKeycode);
    return false;
}

bool Input::isMouseButtonPressed(const MouseCode iMouseCode) {
    if (mus_instance)
        return mus_instance->isMouseButtonPressed_impl(iMouseCode);
    return false;
}

float Input::getMouseX() { return getMousePos().x; }

float Input::getMouseY() { return getMousePos().y; }

glm::vec2 Input::getMousePos() {
    if (mus_instance)
        return mus_instance->getMousePos_impl();
    return {};
}

void Input::injectKey(const KeyCode iKeycode) {
    if (mus_instance)
        mus_instance->injectKey_impl(iKeycode);
}

void Input::injectMouseButton(const MouseCode iMouseCode) {
    if (mus_instance)
        mus_instance->injectMouseButton_impl(iMouseCode);
}

void Input::injectMousePos(const glm::vec2 &iMousePos) {
    if (mus_instance)
        mus_instance->injectMousePos_impl(iMousePos);
}

void Input::resetInjection() {
    if (mus_instance)
        mus_instance->resetInjection_impl();
}

}// namespace owl::input
