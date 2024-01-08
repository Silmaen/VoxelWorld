/**
 * @file AppEvent.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Event.h"
#include <fmt/format.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

namespace owl::event {
/**
 * @brief Window Resize Event.
 */
class OWL_API WindowResizeEvent : public Event {
public:
	/**
	 * @brief Constructor.
	 * @param width_ Window's width.
	 * @param height_ Window's height.
	 */
	WindowResizeEvent(uint32_t width_, uint32_t height_)
		: width(width_), height(height_) {}

	/**
	 * @brief Get window's width.
	 * @return The window's width.
	 */
	[[nodiscard]] uint32_t getWidth() const { return width; }

	/**
	 * @brief Get window's height.
	 * @return The window's height.
	 */
	[[nodiscard]] uint32_t getHeight() const { return height; }

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("WindowResizeEvent: {}, {}", width, height);
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override {
		return fmt::format("WindowResizeEvent");
	}

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static type getStaticType() { return type::WindowResize; }

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
		return category::Application;
	}

private:
	/// New width.
	uint32_t width;
	/// New height.
	uint32_t height;
};

/**
 * @brief Window close Event
 */
class OWL_API WindowCloseEvent : public Event {
public:
	/**
	 * @brief Constructor.
	 */
	WindowCloseEvent() = default;

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("WindowCloseEvent");
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override {
		return fmt::format("WindowCloseEvent");
	}

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static type getStaticType() { return type::WindowClose; }

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
		return category::Application;
	}
};

/**
 * @brief Application Tick Event
 */
class OWL_API AppTickEvent : public Event {
public:
	/**
	 * @brief Constructor.
	 */
	AppTickEvent() = default;

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("AppTickEvent");
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override {
		return fmt::format("AppTickEvent");
	}

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static type getStaticType() { return type::AppTick; }

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
		return category::Application;
	}
};

/**
 * @brief Application Update Event
 */
class OWL_API AppUpdateEvent : public Event {
public:
	/**
	 * @brief Constructor.
	 */
	AppUpdateEvent() = default;

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("AppUpdateEvent");
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override {
		return fmt::format("AppUpdateEvent");
	}

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static type getStaticType() { return type::AppUpdate; }

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
		return category::Application;
	}
};

/**
 * @brief Application Rendering Event
 */
class OWL_API AppRenderEvent : public Event {
public:
	/**
	 * @brief Constructor.
	 */
	AppRenderEvent() = default;

	/**
	 * @brief Get the event as string.
	 * @return String of the event.
	 */
	[[nodiscard]] std::string toString() const override {
		return fmt::format("AppRenderEvent");
	}

	/**
	 * @brief Get the event's name.
	 * @return Event's name.
	 */
	[[nodiscard]] std::string getName() const override {
		return fmt::format("AppRenderEvent");
	}

	/**
	 * @brief Get the event's static type.
	 * @return Event's static type.
	 */
	[[nodiscard]] static type getStaticType() { return type::AppRender; }

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
		return category::Application;
	}
};

}// namespace owl::event

#ifdef __clang__
#pragma clang diagnostic pop
#endif
