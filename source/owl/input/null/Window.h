/**
 * @file Window.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../Window.h"
#include "renderer/GraphContext.h"

/**
 * @brief Namespace for the glfw  elements.
 */
namespace owl::input::null {

/**
 * @brief Class GLFW Window.
 */
class OWL_API Window : public ::owl::input::Window {
public:
	Window(const Window &) = delete;
	Window(Window &&) = delete;
	Window &operator=(const Window &) = delete;
	Window &operator=(Window &&) = delete;

	/**
	 * @brief Default constructor.
	 * @param props The window properties.
	 */
	explicit Window(const Properties &props);

	/**
	 * @brief Destructor.
	 */
	~Window() override;

	/**
	 * @brief Function called at Update Time.
	 */
	void onUpdate() override;

	/**
	 * @brief Get Size attribute of width.
	 * @return The window's width.
	 */
	[[nodiscard]] uint32_t getWidth() const override { return windowData.width; }

	/**
	 * @brief Get Size attribute of height.
	 * @return The window's height.
	 */
	[[nodiscard]] uint32_t getHeight() const override { return windowData.height; }


	/**
	 * @brief Get the type of window manager.
	 * @return The window manager's type.
	 */
	[[nodiscard]] Type getType() const override { return Type::Null; }

	/**
	 * @brief Define the Event Callback function.
	 * @param callback The new callback function.
	 */
	void setEventCallback(const EventCallback &callback) override {
		windowData.eventCallback = callback;
	}
	/**
	 * @brief St the VSync.
	 * @param enabled Should VSync enabled.
	 */
	void setVSync(bool enabled) override;

	/**
	 * @brief Check for VSync.
	 * @return True if VSync enabled.
	 */
	[[nodiscard]] bool isVSync() const override;

	/**
	 * @brief Access to the Native Window.
	 * @return Native window's raw pointer.
	 */
	[[nodiscard]] void *getNativeWindow() const override { return glfwWindow; }

private:
	/**
	 * @brief Initialize the window.
	 * @param props Properties of the window.
	 */
	void init(const Properties &props);

	/**
	 * @brief Terminate the window.
	 */
	void shutdown();
	/// Pointer to the GLFW window.
	void *glfwWindow = nullptr;
	/// Pointer to the Graphic Context.
	uniq<renderer::GraphContext> context;

	/**
	 * @brief Window's data.
	 */
	struct WindowData {
		/// Window's title.
		std::string title{};
		/// Window's width.
		uint32_t width = 0;
		/// Window's height.
		uint32_t height = 0;
		/// Window's VSync property.
		bool VSync = false;
		/// Event Call back.
		EventCallback eventCallback{};
	};

	/// The Window's data.
	WindowData windowData{};
};

}// namespace owl::input::null