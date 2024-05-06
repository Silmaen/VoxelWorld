/**
 * @file Event.cpp
 * @author Silmaen
 * @date 16/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Event.h"

#include "AppEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace owl::event {

Event::~Event() = default;

uint8_t WindowResizeEvent::getCategoryFlags() const { return Category::Application; }
uint8_t WindowCloseEvent::getCategoryFlags() const { return Category::Application; }
uint8_t AppTickEvent::getCategoryFlags() const { return Category::Application; }
uint8_t AppUpdateEvent::getCategoryFlags() const { return Category::Application; }
uint8_t AppRenderEvent::getCategoryFlags() const { return Category::Application; }

uint8_t KeyEvent::getCategoryFlags() const { return Category::Input | Category::Keyboard; }
std::string KeyPressedEvent::getName() const { return fmt::format("KeyPressedEvent"); }
std::string KeyReleasedEvent::getName() const { return fmt::format("KeyReleasedEvent"); }
std::string KeyTypedEvent::getName() const { return fmt::format("KeyTypedEvent"); }

uint8_t MouseMovedEvent::getCategoryFlags() const { return Category::Input | Category::Mouse; }
uint8_t MouseScrolledEvent::getCategoryFlags() const { return Category::Input | Category::Mouse; }
uint8_t MouseButtonEvent::getCategoryFlags() const { return Category::Input | Category::Mouse | Category::MouseButton; }
std::string MouseButtonPressedEvent::getName() const { return fmt::format("MouseButtonPressedEvent"); }
std::string MouseButtonReleasedEvent::getName() const { return fmt::format("MouseButtonReleasedEvent"); }

}// namespace owl::event
