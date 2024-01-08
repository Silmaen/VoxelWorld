/**
 * @file MouseEvent.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Event.h"
#include "input/MouseCode.h"
#include <fmt/format.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

namespace owl::event {

/**
 * @brief Mouse move event.
 */
class OWL_API MouseMovedEvent : public Event {
public:
	/**
	 * @brief Constructor.
	 * @param x X mouse position.
	 * @param y Y mouse position.
	 */
	MouseMovedEvent(const float x, const float y) : mouseX(x), mouseY(y) {}

	/**
	 * @brief Get X mouse position.
	 * @return X mouse position.
	 */
	[[nodiscard]] float getX() const { return mouseX; }

	/**
	 * @brief Get X mouse position.
	 * @return X mouse position.
	 */
	[[nodiscard]] float getY() const { return mouseY; }

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("MouseMovedEvent: {:.1f}, {:.1f}", mouseX, mouseY);
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override {
		return fmt::format("MouseMovedEvent");
	}

	/**
	 * @brief Get the event's category flags.
	 * @return Event's category flags.
	 */
	[[nodiscard]] uint8_t getCategoryFlags() const override {
		return category::Input | category::Mouse;
	}

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static type getStaticType() { return type::MouseMoved; }

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] type getType() const override { return getStaticType(); }

private:
	/// X mouse position.
	float mouseX;
	/// Y mouse position
	float mouseY;
};

/**
 * @brief Event when mouse is scrolled.
 */
class OWL_API MouseScrolledEvent : public Event {
public:
	/**
	 * @brief Constructor.
	 * @param xOffset X wheel offset.
	 * @param yOffset Y wheel offset.
	 */
	MouseScrolledEvent(const float xOffset, const float yOffset)
		: XOffset(xOffset), YOffset(yOffset) {}

	/**
	 * @brief Get X wheel offset.
	 * @return X wheel offset.
	 */
	[[nodiscard]] float getXOff() const { return XOffset; }

	/**
	 * @brief Get Y wheel offset.
	 * @return Y wheel offset.
	 */
	[[nodiscard]] float getYOff() const { return YOffset; }

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("MouseScrolledEvent: {}, {}", getXOff(), getYOff());
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override {
		return fmt::format("MouseScrolledEvent");
	}

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static type getStaticType() { return type::MouseScrolled; }

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] type getType() const override { return getStaticType(); }

	/**
	 * @brief Get the event's category flags.
	 * @return Event's category flags.
	 */
	[[nodiscard]] uint8_t getCategoryFlags() const override {
		return category::Input | category::Mouse;
	}

private:
	/// X wheel offset.
	float XOffset;
	/// Y wheel offset.
	float YOffset;
};

/**
 * @brief Event for mouse button.
 */
class OWL_API MouseButtonEvent : public Event {
public:
	/**
	 * @brief Get the mouse button involved in Event.
	 * @return The mouse button.
	 */
	[[nodiscard]] input::MouseCode GetMouseButton() const { return mouseButton; }

	/**
	 * @brief Get the event's category flags.
	 * @return Event's category flags.
	 */
	[[nodiscard]] uint8_t getCategoryFlags() const override {
		return category::Input | category::Mouse | category::MouseButton;
	}

protected:
	/**
	 * @brief Constructor.
	 * @param button The mouse button.
	 */
	explicit MouseButtonEvent(const input::MouseCode button)
		: mouseButton(button) {}
	/// The mouse button of the event.
	input::MouseCode mouseButton;
};

/**
 * @brief Event for Mouse button pressed.
 */
class OWL_API MouseButtonPressedEvent : public MouseButtonEvent {
public:
	/**
	 * @brief Constructor.
	 * @param button The mouse button.
	 */
	explicit MouseButtonPressedEvent(const input::MouseCode button)
		: MouseButtonEvent(button) {}

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("MouseButtonPressedEvent: {}", mouseButton);
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override {
		return fmt::format("MouseButtonPressedEvent");
	}

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static type getStaticType() { return type::MouseButtonPressed; }

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] type getType() const override { return getStaticType(); }
};

/**
 * @brief Event for mouse button released.
 */
class OWL_API MouseButtonReleasedEvent : public MouseButtonEvent {
public:
	/**
	 * @brief Constructor.
	 * @param button The mouse button.
	 */
	explicit MouseButtonReleasedEvent(const input::MouseCode button)
		: MouseButtonEvent(button) {}

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("MouseButtonReleasedEvent: {}", mouseButton);
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override {
		return fmt::format("MouseButtonReleasedEvent");
	}

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static type getStaticType() {
		return type::MouseButtonReleased;
	}

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] type getType() const override { return getStaticType(); }
};

}// namespace owl::event

#ifdef __clang__
#pragma clang diagnostic pop
#endif
