/**
 * @file Event.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

/**
 * @brief Namespace for Events
 */
namespace owl::event {

/**
 * @brief Type of events.
 */
enum class Type : uint8_t {
	None = 0,/// No type.

	WindowClose,/// Window close.
	WindowResize,/// Window resize.
	WindowFocus,/// Window Focus.
	WindowLostFocus,/// Window lost focus.
	WindowMoved,/// Window moved.

	AppTick,/// Application tick.
	AppUpdate,/// Application update.
	AppRender,/// Application render.

	KeyPressed,/// Keyboard's key pressed.
	KeyReleased,/// Keyboard's key released.
	KeyTyped,/// Keyboard's key typed.

	MouseButtonPressed,/// Mouse's button pressed.
	MouseButtonReleased,/// Mouse's button released.
	MouseMoved,/// Mouse moved.
	MouseScrolled/// Mouse wheel scrolled.
};

/**
 * @brief Event categories.
 */
enum Category : uint8_t {
	None = 0,/// No category.
	Application = 1u,/// Application event.
	Input = 2u,/// Input event.
	Keyboard = 4u,/// Keyboard event.
	Mouse = 8u,/// Mouse event.
	MouseButton = 16u/// Mouse button.
};

/**
 * @brief Abstract Class Event.
 */
class OWL_API Event {
public:
	Event(const Event &) = default;
	Event(Event &&) = default;
	Event &operator=(const Event &) = default;
	Event &operator=(Event &&) = default;
	Event() = default;

	/**
	 * @brief Destructor.
	 */
	virtual ~Event();

	/**
	 * @brief Get the Event type.
	 * @return Event Type.
	 */
	[[nodiscard]] virtual Type getType() const = 0;

	/**
	 * @brief Get the category flags for the Event.
	 * @return All the Event category flags.
	 */
	[[nodiscard]] virtual uint8_t getCategoryFlags() const = 0;

	/**
	 * @brief Get the Event Name.
	 * @return Event names.
	 */
	[[nodiscard]] virtual std::string getName() const = 0;

	/**
	 * @brief Get the Event Name;
	 * @return Event names.
	 */
	[[nodiscard]] virtual std::string toString() const = 0;

	/**
	 * @brief Check if the event belongs to category.
	 * @param[in] iCategory Category to check.
	 * @return True if belongs to category.
	 */
	[[nodiscard]] bool isInCategory(const Category &iCategory) const { return (getCategoryFlags() & iCategory) != 0; }

	/// If event already handled.
	bool handled = false;
};

/**
 * @brief Event Dispatcher class.
 */
class EventDispatcher {
public:
	/**
	 * @brief Constructor.
	 * @param[in,out] ioDispatchEvent Event to dispatch.
	 */
	explicit EventDispatcher(Event &ioDispatchEvent) : m_event(ioDispatchEvent) {}

	/**
	 * @brief Dispatching function.
	 * @tparam T EventType.
	 * @tparam F Function type (will be deduced by the compiler).
	 * @param[in] iFunc The Function.
	 * @return True if succeeded.
	 */
	template<typename T, typename F>
	bool dispatch(const F &iFunc) {
		if (m_event.getType() == T::getStaticType()) {
			m_event.handled |= iFunc(static_cast<T &>(m_event));
			return true;
		}
		return false;
	}

private:
	/// The event.
	Event &m_event;
};

}// namespace owl::event
