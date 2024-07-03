/**
 * @file Window.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../Window.h"

/**
 * @brief Namespace for the glfw  elements.
 */
namespace owl::input::null {

/**
 * @brief GLFW implementation of window.
 */
class OWL_API Window final : public ::owl::input::Window {
public:
	Window(const Window &) = delete;
	Window(Window &&) = delete;
	Window &operator=(const Window &) = delete;
	Window &operator=(Window &&) = delete;

	/**
	 * @brief Default constructor.
	 * @param[in] iProps The window properties.
	 */
	explicit Window(const Properties &iProps);

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
	[[nodiscard]] uint32_t getWidth() const override { return m_windowData.m_size.x(); }

	/**
	 * @brief Get Size attribute of height.
	 * @return The window's height.
	 */
	[[nodiscard]] uint32_t getHeight() const override { return m_windowData.m_size.y(); }

	/**
	 * @brief Access to texture's size.
	 * @return Texture's size.
	 */
	[[nodiscard]] const math::vec2ui& getSize() const override { return m_windowData.m_size; }

	/**
	 * @brief Get the type of window manager.
	 * @return The window manager's type.
	 */
	[[nodiscard]] Type getType() const override { return Type::Null; }

	/**
	 * @brief Define the Event Callback function.
	 * @param[in] iCallback The new callback function.
	 */
	void setEventCallback(const EventCallback &iCallback) override { m_windowData.m_eventCallback = iCallback; }
	/**
	 * @brief St the VSync.
	 * @param[in] iEnabled Should VSync enabled.
	 */
	void setVSync(bool iEnabled) override;

	/**
	 * @brief Check for VSync.
	 * @return True if VSync enabled.
	 */
	[[nodiscard]] bool isVSync() const override;

	/**
	 * @brief Access to the Native Window.
	 * @return Native window's raw pointer.
	 */
	[[nodiscard]] void *getNativeWindow() const override { return mp_glfwWindow; }

	/**
	 * @brief Terminate the window.
	 */
	void shutdown() override;

private:
	/**
	 * @brief Initialize the window.
	 * @param[in] iProps Properties of the window.
	 */
	void init(const Properties &iProps);

	/// Pointer to the GLFW window.
	void *mp_glfwWindow = nullptr;

	/**
	 * @brief Window's data.
	 */
	struct WindowData {
		/// Window's title.
		std::string m_title;
		/// Window's size.
		math::vec2ui m_size {0,0};
		/// Window's VSync property.
		bool m_VSync = false;
		/// Event Call back.
		EventCallback m_eventCallback;
	};

	/// The Window's data.
	WindowData m_windowData{};
};

}// namespace owl::input::null
