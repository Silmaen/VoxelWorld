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
#include <fmt/format.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

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
	[[nodiscard]] input::KeyCode getKeyCode() const { return keyCode; }

	/**
	 * @brief Get the event's category flags.
	 * @return Event's category flags.
	 */
	[[nodiscard]] uint8_t getCategoryFlags() const override {
		return category::Input | category::Keyboard;
	}

protected:
	explicit KeyEvent(const input::KeyCode keycode) : keyCode(keycode) {}
	/// Key code.
	input::KeyCode keyCode;
};

/**
 * @brief Event of key presse
 */
class OWL_API KeyPressedEvent : public KeyEvent {
public:
	/**
	 * @brief Constructor.
	 * @param keycode Code of the key pressed.
	 * @param repeatCount_ Number of repeat.
	 */
	explicit KeyPressedEvent(const input::KeyCode keycode, uint16_t repeatCount_)
		: KeyEvent(keycode), repeatCount(repeatCount_) {}

	/**
	 * @brief Check if the key is repeated
	 * @return True if repeated
	 */
	[[nodiscard]] bool getRepeatCount() const { return repeatCount; }

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("KeyPressedEvent: {} (repeat = {})", keyCode, repeatCount);
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override {
		return fmt::format("KeyPressedEvent");
	}

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static type getStaticType() { return type::KeyPressed; }

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] type getType() const override { return getStaticType(); }

private:
	/// Counter of repeat.
	uint16_t repeatCount;
};

/**
 * @brief Event of key released
 */
class OWL_API KeyReleasedEvent : public KeyEvent {
public:
	/**
	 * @brief Constructor.
	 * @param keycode Code of the key released.
	 */
	explicit KeyReleasedEvent(const input::KeyCode keycode) : KeyEvent(keycode) {}

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("KeyReleasedEvent: {}", keyCode);
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override {
		return fmt::format("KeyReleasedEvent");
	}

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static type getStaticType() { return type::KeyReleased; }

	/**
	 * @brief Get the event's type.
	 * @return Event's type.
	 */
	[[nodiscard]] type getType() const override { return getStaticType(); }
};

/**
 * @brief Event of key typed
 */
class KeyTypedEvent : public KeyEvent {
public:
	/**
	 * @brief Constructor.
	 * @param keycode Code of the key typed.
	 */
	explicit KeyTypedEvent(const input::KeyCode keycode) : KeyEvent(keycode) {}

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("KeyTypedEvent: {}", keyCode);
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override {
		return fmt::format("KeyTypedEvent");
	}

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static type getStaticType() { return type::KeyTyped; }

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
