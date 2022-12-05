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

enum class type {
  None = 0,

  WindowClose,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,

  AppTick,
  AppUpdate,
  AppRender,

  KeyPressed,
  KeyReleased,
  KeyTyped,

  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

enum category {
  None = 0,
  Application = 1,
  Input = 2,
  Keyboard = 4,
  Mouse = 8,
  MouseButton = 16
};

/**
 * @brief Abstract Class Event
 */
class OWL_API Event {
public:
  Event(const Event &) = default;
  Event(Event &&) = default;
  Event &operator=(const Event &) = default;
  Event &operator=(Event &&) = default;
  /**
   * @brief Default constructor.
   */
  Event() = default;
  /**
   * @brief Destructor.
   */
  virtual ~Event() = default;

  /**
   * @brief Get the Event type
   * @return Event Type
   */
  [[nodiscard]] virtual type getType() const = 0;
  /**
   * @brief Get the category flags for the Event
   * @return All the Event category flags
   */
  [[nodiscard]] virtual uint8_t getCategoryFlags() const = 0;
  /**
   * @brief Get the Event Name
   * @return Event names
   */
  [[nodiscard]] virtual std::string getName() const = 0;
  /**
   * @brief Get the Event Name
   * @return Event names
   */
  [[nodiscard]] virtual std::string toString() const = 0;

  /**
   * @brief Check if the event belongs to category
   * @param cat Category to check
   * @return True if belongs to category
   */
  [[nodiscard]] bool isinCategory(const category &cat) const {
    return (getCategoryFlags() & cat) != 0;
  }
  /// If event already handled
  bool handled = false;
};

/**
 * @brief Event Dispatcher class
 */
class EventDispatcher {
public:
  /**
   * @brief Constructor
   * @param dispatchEvent Event to dispatch
   */
  explicit EventDispatcher(Event &dispatchEvent) : event(dispatchEvent) {}

  /**
   * @brief Dispatching function
   * @tparam T EventType
   * @tparam F Function type (will be deduced by the compiler)
   * @param func The Function
   * @return True if succeeded
   */
  template <typename T, typename F> bool dispatch(const F &func) {
    if (event.getType() == T::getStaticType()) {
      event.handled |= func(static_cast<T &>(event));
      return true;
    }
    return false;
  }

private:
  /// The event
  Event &event;
};

} // namespace owl::event
