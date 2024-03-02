/**
 * @file KeyEvent.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Event.h"
#include "input/KeyCodes.h"

namespace owl::event {

/**
 * @brief Semi-abstracted class for all Keyboard events.
 */
class OWL_API KeyEvent : public Event {
public:
	/**
	 * @brief Get the Key code.
	 * @return The Event Key code.
	 */
	[[nodiscard]] input::KeyCode getKeyCode() const { return m_keyCode; }

	/**
	 * @brief Get the event's category flags.
	 * @return Event's category flags.
	 */
	[[nodiscard]] uint8_t getCategoryFlags() const override { return Category::Input | Category::Keyboard; }

protected:
	explicit KeyEvent(const input::KeyCode iKeyCode) : m_keyCode(iKeyCode) {}
	/// Key code.
	input::KeyCode m_keyCode;
};

/**
 * @brief Event of key presse
 */
class OWL_API KeyPressedEvent final : public KeyEvent {
public:
	/**
	 * @brief Constructor.
	 * @param[in] iKeyCode Code of the key pressed.
	 * @param[in] iRepeatCount Number of repeat.
	 */
	explicit KeyPressedEvent(const input::KeyCode iKeyCode, const uint16_t iRepeatCount)
		: KeyEvent(iKeyCode), m_repeatCount(iRepeatCount) {}

	/**
	 * @brief Check if the key is repeated
	 * @return True if repeated
	 */
	[[nodiscard]] bool getRepeatCount() const { return m_repeatCount; }

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("KeyPressedEvent: {} (repeat = {})", m_keyCode, m_repeatCount);
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override { return fmt::format("KeyPressedEvent"); }

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static Type getStaticType() { return Type::KeyPressed; }

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] Type getType() const override { return getStaticType(); }

private:
	/// Counter of repeat.
	uint16_t m_repeatCount;
};

/**
 * @brief Event of key released
 */
class OWL_API KeyReleasedEvent final : public KeyEvent {
public:
	/**
	 * @brief Constructor.
	 * @param[in] iKeyCode Code of the key released.
	 */
	explicit KeyReleasedEvent(const input::KeyCode iKeyCode) : KeyEvent(iKeyCode) {}

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override { return fmt::format("KeyReleasedEvent: {}", m_keyCode); }

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override { return fmt::format("KeyReleasedEvent"); }

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static Type getStaticType() { return Type::KeyReleased; }

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] Type getType() const override { return getStaticType(); }
};

/**
 * @brief Event of key typed
 */
class KeyTypedEvent final : public KeyEvent {
public:
	/**
	 * @brief Constructor.
	 * @param[in] iKeyCode Code of the key typed.
	 */
	explicit KeyTypedEvent(const input::KeyCode iKeyCode) : KeyEvent(iKeyCode) {}

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override { return fmt::format("KeyTypedEvent: {}", m_keyCode); }

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override { return fmt::format("KeyTypedEvent"); }

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static Type getStaticType() { return Type::KeyTyped; }

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] Type getType() const override { return getStaticType(); }
};

}// namespace owl::event
