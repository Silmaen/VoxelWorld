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
 * @brief Semi-abstracted class for all Keyboard events
 */
class OWL_API KeyEvent : public Event {
public:
	/**
	 * @brief Get the Key code
	 * @return The Event Key code
	 */
	[[nodiscard]] input::KeyCode getKeyCode() const { return keyCode; }

	[[nodiscard]] uint8_t getCategoryFlags() const override {
		return category::Input | category::Keyboard;
	}

protected:
	explicit KeyEvent(const input::KeyCode keycode) : keyCode(keycode) {}
	/// Key code
	input::KeyCode keyCode;
};

/**
 * @brief Event of key presse
 */
class OWL_API KeyPressedEvent : public KeyEvent {
public:
	explicit KeyPressedEvent(const input::KeyCode keycode, uint16_t  repeatCount_)
		: KeyEvent(keycode), repeatCount(repeatCount_) {}

	/**
	 * @brief Check if the key is repeated
	 * @return True if repeated
	 */
	[[nodiscard]] bool getRepeatCount() const { return repeatCount; }

	[[nodiscard]] std::string toString() const override {
		return fmt::format("KeyPressedEvent: {} (repeat = {})", keyCode, repeatCount);
	}
	[[nodiscard]] std::string getName() const override {
		return fmt::format("KeyPressedEvent");
	}

	[[nodiscard]] static type getStaticType() { return type::KeyPressed; }
	[[nodiscard]] type getType() const override { return getStaticType(); }

private:
	uint16_t  repeatCount;
};

/**
 * @brief Event of key released
 */
class OWL_API KeyReleasedEvent : public KeyEvent {
public:
	explicit KeyReleasedEvent(const input::KeyCode keycode) : KeyEvent(keycode) {}

	[[nodiscard]] std::string toString() const override {
		return fmt::format("KeyReleasedEvent: {}", keyCode);
	}
	[[nodiscard]] std::string getName() const override {
		return fmt::format("KeyReleasedEvent");
	}

	[[nodiscard]] static type getStaticType() { return type::KeyReleased; }
	[[nodiscard]] type getType() const override { return getStaticType(); }
};

/**
 * @brief Event of key typed
 */
class KeyTypedEvent : public KeyEvent {
public:
	explicit KeyTypedEvent(const input::KeyCode keycode) : KeyEvent(keycode) {}

	[[nodiscard]] std::string toString() const override {
		return fmt::format("KeyTypedEvent: {}", keyCode);
	}
	[[nodiscard]] std::string getName() const override {
		return fmt::format("KeyTypedEvent");
	}
	[[nodiscard]] static type getStaticType() { return type::KeyTyped; }
	[[nodiscard]] type getType() const override { return getStaticType(); }
};

}// namespace owl::event

#ifdef __clang__
#pragma clang diagnostic pop
#endif
