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

namespace owl::event {
/**
 * @brief Mouse move event.
 */
class OWL_API MouseMovedEvent final : public Event {
public:
	/**
	 * @brief Constructor.
	 * @param[in] iX X mouse position.
	 * @param[in] iY Y mouse position.
	 */
	MouseMovedEvent(const float iX, const float iY) : m_mouseX(iX), m_mouseY(iY) {}

	/**
	 * @brief Get X mouse position.
	 * @return X mouse position.
	 */
	[[nodiscard]] float getX() const { return m_mouseX; }

	/**
	 * @brief Get X mouse position.
	 * @return X mouse position.
	 */
	[[nodiscard]] float getY() const { return m_mouseY; }

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("MouseMovedEvent: {:.1f}, {:.1f}", m_mouseX, m_mouseY);
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override { return fmt::format("MouseMovedEvent"); }

	/**
	 * @brief Get the event's category flags.
	 * @return Event's category flags.
	 */
	[[nodiscard]] uint8_t getCategoryFlags() const override { return Category::Input | Category::Mouse; }

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static Type getStaticType() { return Type::MouseMoved; }

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] Type getType() const override { return getStaticType(); }

private:
	/// X mouse position.
	float m_mouseX;
	/// Y mouse position
	float m_mouseY;
};

/**
 * @brief Event when mouse is scrolled.
 */
class OWL_API MouseScrolledEvent final : public Event {
public:
	/**
	 * @brief Constructor.
	 * @param[in] iXOffset X wheel offset.
	 * @param[in] iYOffset Y wheel offset.
	 */
	MouseScrolledEvent(const float iXOffset, const float iYOffset)
		: m_xOffset(iXOffset), m_yOffset(iYOffset) {}

	/**
	 * @brief Get X wheel offset.
	 * @return X wheel offset.
	 */
	[[nodiscard]] float getXOff() const { return m_xOffset; }

	/**
	 * @brief Get Y wheel offset.
	 * @return Y wheel offset.
	 */
	[[nodiscard]] float getYOff() const { return m_yOffset; }

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
	[[nodiscard]] std::string getName() const override { return fmt::format("MouseScrolledEvent"); }

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static Type getStaticType() { return Type::MouseScrolled; }

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] Type getType() const override { return getStaticType(); }

	/**
	 * @brief Get the event's category flags.
	 * @return Event's category flags.
	 */
	[[nodiscard]] uint8_t getCategoryFlags() const override { return Category::Input | Category::Mouse; }

private:
	/// X wheel offset.
	float m_xOffset;
	/// Y wheel offset.
	float m_yOffset;
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
	[[nodiscard]] input::MouseCode getMouseButton() const { return m_mouseButton; }

	/**
	 * @brief Get the event's category flags.
	 * @return Event's category flags.
	 */
	[[nodiscard]] uint8_t getCategoryFlags() const override {
		return Category::Input | Category::Mouse | Category::MouseButton;
	}

protected:
	/**
	 * @brief Constructor.
	 * @param[in] iXOffset The mouse button.
	 */
	explicit MouseButtonEvent(const input::MouseCode iXOffset)
		: m_mouseButton(iXOffset) {}

	/// The mouse button of the event.
	input::MouseCode m_mouseButton;
};

/**
 * @brief Event for Mouse button pressed.
 */
class OWL_API MouseButtonPressedEvent final : public MouseButtonEvent {
public:
	/**
	 * @brief Constructor.
	 * @param[in] iButton The mouse button.
	 */
	explicit MouseButtonPressedEvent(const input::MouseCode iButton)
		: MouseButtonEvent(iButton) {}

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("MouseButtonPressedEvent: {}", m_mouseButton);
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override { return fmt::format("MouseButtonPressedEvent"); }

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static Type getStaticType() { return Type::MouseButtonPressed; }

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] Type getType() const override { return getStaticType(); }
};

/**
 * @brief Event for mouse button released.
 */
class OWL_API MouseButtonReleasedEvent final : public MouseButtonEvent {
public:
	/**
	 * @brief Constructor.
	 * @param[in] iButton The mouse button.
	 */
	explicit MouseButtonReleasedEvent(const input::MouseCode iButton)
		: MouseButtonEvent(iButton) {}

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("MouseButtonReleasedEvent: {}", m_mouseButton);
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override { return fmt::format("MouseButtonReleasedEvent"); }

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static Type getStaticType() { return Type::MouseButtonReleased; }

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] Type getType() const override { return getStaticType(); }
};
} // namespace owl::event
