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
 * @brief Window Resize Event
 */
class OWL_API WindowResizeEvent : public Event {
public:
	WindowResizeEvent(uint32_t width_, uint32_t height_)
		: width(width_), height(height_) {}

	[[nodiscard]] uint32_t getWidth() const { return width; }

	[[nodiscard]] uint32_t getHeight() const { return height; }

	[[nodiscard]] std::string toString() const override {
		return fmt::format("WindowResizeEvent: {}, {}", width, height);
	}
	[[nodiscard]] std::string getName() const override {
		return fmt::format("WindowResizeEvent");
	}

	[[nodiscard]] static type getStaticType() { return type::WindowResize; }
	[[nodiscard]] type getType() const override { return getStaticType(); }

	[[nodiscard]] uint8_t getCategoryFlags() const override {
		return category::Application;
	}

private:
	uint32_t width;
	uint32_t height;
};

/**
 * @brief Window close Event
 */
class OWL_API WindowCloseEvent : public Event {
public:
	WindowCloseEvent() = default;

	[[nodiscard]] std::string toString() const override {
		return fmt::format("WindowCloseEvent");
	}
	[[nodiscard]] std::string getName() const override {
		return fmt::format("WindowCloseEvent");
	}

	[[nodiscard]] static type getStaticType() { return type::WindowClose; }
	[[nodiscard]] type getType() const override { return getStaticType(); }
	[[nodiscard]] uint8_t getCategoryFlags() const override {
		return category::Application;
	}
};

/**
 * @brief Application Tick Event
 */
class OWL_API AppTickEvent : public Event {
public:
	AppTickEvent() = default;

	[[nodiscard]] std::string toString() const override {
		return fmt::format("AppTickEvent");
	}
	[[nodiscard]] std::string getName() const override {
		return fmt::format("AppTickEvent");
	}
	[[nodiscard]] static type getStaticType() { return type::AppTick; }
	[[nodiscard]] type getType() const override { return getStaticType(); }
	[[nodiscard]] uint8_t getCategoryFlags() const override {
		return category::Application;
	}
};

/**
 * @brief Application Update Event
 */
class OWL_API AppUpdateEvent : public Event {
public:
	AppUpdateEvent() = default;

	[[nodiscard]] std::string toString() const override {
		return fmt::format("AppUpdateEvent");
	}
	[[nodiscard]] std::string getName() const override {
		return fmt::format("AppUpdateEvent");
	}
	[[nodiscard]] static type getStaticType() { return type::AppUpdate; }
	[[nodiscard]] type getType() const override { return getStaticType(); }
	[[nodiscard]] uint8_t getCategoryFlags() const override {
		return category::Application;
	}
};

/**
 * @brief Application Rendering Event
 */
class OWL_API AppRenderEvent : public Event {
public:
	AppRenderEvent() = default;
	[[nodiscard]] std::string toString() const override {
		return fmt::format("AppRenderEvent");
	}
	[[nodiscard]] std::string getName() const override {
		return fmt::format("AppRenderEvent");
	}

	[[nodiscard]] static type getStaticType() { return type::AppRender; }
	[[nodiscard]] type getType() const override { return getStaticType(); }
	[[nodiscard]] uint8_t getCategoryFlags() const override {
		return category::Application;
	}
};

}// namespace owl::event

#ifdef __clang__
#pragma clang diagnostic pop
#endif
