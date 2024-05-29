/**
 * @file Window.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/Core.h"
#include "event/Event.h"
#include "math/sizingTypes.h"
#include "renderer/GraphContext.h"

namespace owl::input {

/**
 * @brief Types of input Manager.
 */
enum struct Type : uint8_t {
	Null,/// Windows null
	GLFW/// Windows managed by Glfw library.
};

/**
 * @brief Structure holding base windows properties.
 */
struct Properties {
	/// Type of Window manager.
	Type winType = Type::GLFW;
	/// Window title.
	std::string title = "Owl Engine";
	/// Path to the icons in the assets.
	std::string iconPath;
	/// Width.
	uint32_t width = 1600;
	/// Height.
	uint32_t height = 900;
};

/**
 * @brief Abstract Window class.
 */
class OWL_API Window {
public:
	/// Call back function's type.
	using EventCallback = std::function<void(event::Event &)>;

	Window() = default;
	Window(const Window &) = delete;
	Window(Window &&) = delete;
	Window &operator=(const Window &) = delete;
	Window &operator=(Window &&) = delete;

	/**
	 * @brief Destructor.
	 */
	virtual ~Window();

	/**
	 * @brief Function called at Update Time.
	 */
	virtual void onUpdate() = 0;

	/**
	 * @brief Get Size attribute of width.
	 * @return The window's width.
	 */
	[[nodiscard]] virtual uint32_t getWidth() const = 0;

	/**
	 * @brief Get Size attribute of height.
	 * @return The window's height.
	 */
	[[nodiscard]] virtual uint32_t getHeight() const = 0;

	/**
	 * @brief Access to texture's size.
	 * @return Texture's size.
	 */
	[[nodiscard]] virtual math::FrameSize getSize() const = 0;

	/**
	 * @brief Get the type of window manager.
	 * @return The window manager's type.
	 */
	[[nodiscard]] virtual Type getType() const = 0;

	/**
	 * @brief Define the Event Callback function.
	 * @param[in] iCallback The new callback function.
	 */
	virtual void setEventCallback(const EventCallback &iCallback) = 0;

	/**
	 * @brief St the VSync.
	 * @param[in] iEnabled Should VSync enabled.
	 */
	virtual void setVSync(bool iEnabled) = 0;

	/**
	 * @brief Check for VSync.
	 * @return True if VSync enabled.
	 */
	[[nodiscard]] virtual bool isVSync() const = 0;

	/**
	 * @brief Access to the Native Window.
	 * @return Native window's raw pointer.
	 */
	[[nodiscard]] virtual void *getNativeWindow() const = 0;

	/**
	 * @brief Helper for Window creation.
	 * @param[in] iProps The window properties.
	 * @return Pointer to the window.
	 */
	static uniq<Window> create(const Properties &iProps = Properties());

	/**
	 * @brief Access to the graph context.
	 * @return Graph context.
	 */
	[[nodiscard]] renderer::GraphContext *getGraphContext() const { return mu_context.get(); }

	/**
	 * @brief Terminate the window.
	 */
	virtual void shutdown() = 0;

protected:
	/// Pointer to the Graphic Context.
	uniq<renderer::GraphContext> mu_context;
};

}// namespace owl::input
