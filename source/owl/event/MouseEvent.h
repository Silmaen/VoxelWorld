/**
 * @file MouseEvent.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Event.h"
#include "core/MouseCode.h"
#include <fmt/format.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

namespace owl::event {

class OWL_API MouseMovedEvent : public Event {
public:
  MouseMovedEvent(const float x, const float y) : mouseX(x), mouseY(y) {}

  [[nodiscard]] float getX() const { return mouseX; }
  [[nodiscard]] float getY() const { return mouseY; }

  [[nodiscard]] std::string toString() const override {
    return fmt::format("MouseMovedEvent: {}, {}", getX(), getY());
  }
  [[nodiscard]] std::string getName() const override {
    return fmt::format("MouseMovedEvent");
  }

  [[nodiscard]] uint8_t getCategoryFlags() const override {
    return category::Input | category::Mouse;
  }
  [[nodiscard]] static type getStaticType() { return type::MouseMoved; }
  [[nodiscard]] type getType() const override { return getStaticType(); }

private:
  float mouseX;
  float mouseY;
};

class OWL_API MouseScrolledEvent : public Event {
public:
  MouseScrolledEvent(const float xOffset, const float yOffset)
      : XOffset(xOffset), YOffset(yOffset) {}

  [[nodiscard]] float getXOff() const { return XOffset; }
  [[nodiscard]] float getYOff() const { return YOffset; }

  [[nodiscard]] std::string toString() const override {
    return fmt::format("MouseScrolledEvent: {}, {}", getXOff(), getYOff());
  }
  [[nodiscard]] std::string getName() const override {
    return fmt::format("MouseScrolledEvent");
  }
  [[nodiscard]] static type getStaticType() { return type::MouseScrolled; }
  [[nodiscard]] type getType() const override { return getStaticType(); }
  [[nodiscard]] uint8_t getCategoryFlags() const override {
    return category::Input | category::Mouse;
  }

private:
  float XOffset;
  float YOffset;
};

class OWL_API MouseButtonEvent : public Event {
public:
  [[nodiscard]] core::MouseCode GetMouseButton() const { return mouseButton; }

  [[nodiscard]] uint8_t getCategoryFlags() const override {
    return category::Input | category::Mouse | category::MouseButton;
  }

protected:
  explicit MouseButtonEvent(const core::MouseCode button)
      : mouseButton(button) {}

  core::MouseCode mouseButton;
};

class OWL_API MouseButtonPressedEvent : public MouseButtonEvent {
public:
  explicit MouseButtonPressedEvent(const core::MouseCode button)
      : MouseButtonEvent(button) {}

  [[nodiscard]] std::string toString() const override {
    return fmt::format("MouseButtonPressedEvent: {}", mouseButton);
  }
  [[nodiscard]] std::string getName() const override {
    return fmt::format("MouseButtonPressedEvent");
  }
  [[nodiscard]] static type getStaticType() { return type::MouseButtonPressed; }
  [[nodiscard]] type getType() const override { return getStaticType(); }
};

class OWL_API MouseButtonReleasedEvent : public MouseButtonEvent {
public:
  explicit MouseButtonReleasedEvent(const core::MouseCode button)
      : MouseButtonEvent(button) {}

  [[nodiscard]] std::string toString() const override {
    return fmt::format("MouseButtonReleasedEvent: {}", mouseButton);
  }
  [[nodiscard]] std::string getName() const override {
    return fmt::format("MouseButtonReleasedEvent");
  }
  [[nodiscard]] static type getStaticType() {
    return type::MouseButtonReleased;
  }
  [[nodiscard]] type getType() const override { return getStaticType(); }
};

} // namespace owl::event

#ifdef __clang__
#pragma clang diagnostic pop
#endif
