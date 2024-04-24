/**
 * @file AppEvent.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <math/sizingTypes.h>

#include "Event.h"

namespace owl::event
{
	/**
	 * @brief Window Resize Event.
	 */
	class OWL_API WindowResizeEvent final : public Event
	{
	public:
		/**
		 * @brief Constructor.
		 * @param[in] iWidth Window's width.
		 * @param[in] iHeight Window's height.
		 */
		WindowResizeEvent(const uint32_t iWidth, const uint32_t iHeight)
			: m_size{iWidth, iHeight}
		{
		}

		/**
		 * @brief Constructor.
		 * @param[in] iSize Window's size.
		 */
		explicit WindowResizeEvent(const math::FrameSize& iSize)
			: m_size{iSize}
		{
		}

		/**
		 * @brief Get window's width.
		 * @return The window's width.
		 */
		[[nodiscard]] uint32_t getWidth() const { return m_size.width(); }

		/**
		 * @brief Get window's height.
		 * @return The window's height.
		 */
		[[nodiscard]] uint32_t getHeight() const { return m_size.height(); }

		/**
		 * @brief Get window's size.
		 * @return The window's size.
		 */
		[[nodiscard]] const math::FrameSize& getSize() const { return m_size; }

		/**
		 * @brief Get the event as string.
		 * @return String of the event.
		 */
		[[nodiscard]] std::string toString() const override
		{
			return fmt::format("WindowResizeEvent: {}, {}", m_size.width(), m_size.height());
		}

		/**
		 * @brief Get the event's name.
		 * @return Event's name.
		 */
		[[nodiscard]] std::string getName() const override { return fmt::format("WindowResizeEvent"); }

		/**
		 * @brief Get the event's static type.
		 * @return Event's static type.
		 */
		[[nodiscard]] static Type getStaticType() { return Type::WindowResize; }

		/**
		 * @brief Get the event's type.
		 * @return Event's type.
		 */
		[[nodiscard]] Type getType() const override { return getStaticType(); }

		/**
		 * @brief Get the event's category flags.
		 * @return Event's category flags.
		 */
		[[nodiscard]] uint8_t getCategoryFlags() const override { return Category::Application; }

	private:
		/// New size.
		math::FrameSize m_size;
	};

	/**
	 * @brief Window close Event
	 */
	class OWL_API WindowCloseEvent final : public Event
	{
	public:
		/**
		 * @brief Constructor.
		 */
		WindowCloseEvent() = default;

		/**
		 * @brief Get the event as string.
		 * @return String of the event.
		 */
		[[nodiscard]] std::string toString() const override { return fmt::format("WindowCloseEvent"); }

		/**
		 * @brief Get the event's name.
		 * @return Event's name.
		 */
		[[nodiscard]] std::string getName() const override { return fmt::format("WindowCloseEvent"); }

		/**
		 * @brief Get the event's static type.
		 * @return Event's static type.
		 */
		[[nodiscard]] static Type getStaticType() { return Type::WindowClose; }

		/**
		 * @brief Get the event's type.
		 * @return Event's type.
		 */
		[[nodiscard]] Type getType() const override { return getStaticType(); }

		/**
		 * @brief Get the event's category flags.
		 * @return Event's category flags.
		 */
		[[nodiscard]] uint8_t getCategoryFlags() const override { return Category::Application; }
	};

	/**
	 * @brief Application Tick Event
	 */
	class OWL_API AppTickEvent final : public Event
	{
	public:
		/**
		 * @brief Constructor.
		 */
		AppTickEvent() = default;

		/**
		 * @brief Get the event as string.
		 * @return String of the event.
		 */
		[[nodiscard]] std::string toString() const override { return fmt::format("AppTickEvent"); }

		/**
		 * @brief Get the event's name.
		 * @return Event's name.
		 */
		[[nodiscard]] std::string getName() const override { return fmt::format("AppTickEvent"); }

		/**
		 * @brief Get the event's static type.
		 * @return Event's static type.
		 */
		[[nodiscard]] static Type getStaticType() { return Type::AppTick; }

		/**
		 * @brief Get the event's type.
		 * @return Event's type.
		 */
		[[nodiscard]] Type getType() const override { return getStaticType(); }

		/**
		 * @brief Get the event's category flags.
		 * @return Event's category flags.
		 */
		[[nodiscard]] uint8_t getCategoryFlags() const override { return Category::Application; }
	};

	/**
	 * @brief Application Update Event
	 */
	class OWL_API AppUpdateEvent final : public Event
	{
	public:
		/**
		 * @brief Constructor.
		 */
		AppUpdateEvent() = default;

		/**
		 * @brief Get the event as string.
		 * @return String of the event.
		 */
		[[nodiscard]] std::string toString() const override { return fmt::format("AppUpdateEvent"); }

		/**
		 * @brief Get the event's name.
		 * @return Event's name.
		 */
		[[nodiscard]] std::string getName() const override { return fmt::format("AppUpdateEvent"); }

		/**
		 * @brief Get the event's static type.
		 * @return Event's static type.
		 */
		[[nodiscard]] static Type getStaticType() { return Type::AppUpdate; }

		/**
		 * @brief Get the event's type.
		 * @return Event's type.
		 */
		[[nodiscard]] Type getType() const override { return getStaticType(); }

		/**
		 * @brief Get the event's category flags.
		 * @return Event's category flags.
		 */
		[[nodiscard]] uint8_t getCategoryFlags() const override { return Category::Application; }
	};

	/**
	 * @brief Application Rendering Event
	 */
	class OWL_API AppRenderEvent final : public Event
	{
	public:
		/**
		 * @brief Constructor.
		 */
		AppRenderEvent() = default;

		/**
		 * @brief Get the event as string.
		 * @return String of the event.
		 */
		[[nodiscard]] std::string toString() const override { return fmt::format("AppRenderEvent"); }

		/**
		 * @brief Get the event's name.
		 * @return Event's name.
		 */
		[[nodiscard]] std::string getName() const override { return fmt::format("AppRenderEvent"); }

		/**
		 * @brief Get the event's static type.
		 * @return Event's static type.
		 */
		[[nodiscard]] static Type getStaticType() { return Type::AppRender; }

		/**
		 * @brief Get the event's type.
		 * @return Event's type.
		 */
		[[nodiscard]] Type getType() const override { return getStaticType(); }

		/**
		 * @brief Get the event's category flags.
		 * @return Event's category flags.
		 */
		[[nodiscard]] uint8_t getCategoryFlags() const override { return Category::Application; }
	};
} // namespace owl::event
