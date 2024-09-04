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

auto WindowResizeEvent::getCategoryFlags() const -> uint8_t { return Category::Application; }
auto WindowCloseEvent::getCategoryFlags() const -> uint8_t { return Category::Application; }
auto AppTickEvent::getCategoryFlags() const -> uint8_t { return Category::Application; }
auto AppUpdateEvent::getCategoryFlags() const -> uint8_t { return Category::Application; }
auto AppRenderEvent::getCategoryFlags() const -> uint8_t { return Category::Application; }

auto KeyEvent::getCategoryFlags() const -> uint8_t { return Category::Input | Category::Keyboard; }
auto KeyPressedEvent::getName() const -> std::string { return fmt::format("KeyPressedEvent"); }
auto KeyReleasedEvent::getName() const -> std::string { return fmt::format("KeyReleasedEvent"); }
auto KeyTypedEvent::getName() const -> std::string { return fmt::format("KeyTypedEvent"); }

auto MouseMovedEvent::getCategoryFlags() const -> uint8_t { return Category::Input | Category::Mouse; }
auto MouseScrolledEvent::getCategoryFlags() const -> uint8_t { return Category::Input | Category::Mouse; }
auto MouseButtonEvent::getCategoryFlags() const -> uint8_t {
	return static_cast<uint8_t>(Category::Input | Category::Mouse) | static_cast<uint8_t>(Category::MouseButton);
}
auto MouseButtonPressedEvent::getName() const -> std::string { return fmt::format("MouseButtonPressedEvent"); }
auto MouseButtonReleasedEvent::getName() const -> std::string { return fmt::format("MouseButtonReleasedEvent"); }

}// namespace owl::event
