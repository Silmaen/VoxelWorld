/**
 * @file Application.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <filesystem>

#include "Log.h"
#include "Timestep.h"
#include "event/AppEvent.h"
#include "gui/ImGuiLayer.h"
#include "layer/LayerStack.h"
#include "window/Window.h"

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv List of argument
 * @return Error code
 */
int main(int argc, char *argv[]);

namespace owl::core {

/**
 * @brief Class Application
 */
class OWL_API Application {
public:
	Application(const Application &) = delete;
	Application(Application &&) = delete;
	Application &operator=(const Application &) = delete;
	Application &operator=(Application &&) = delete;
	/**
	 * @brief Default constructor.
	 */
	Application();
	/**
	 * @brief Access to Application instance
	 * @return Single instance of application
	 */
	static Application &get() { return *instance; }
	/**
	 * @brief Destructor.
	 */
	virtual ~Application();
	/**
	 * @brief Runs the application
	 */
	void run();
	/**
	 * @brief Event Callback function
	 * @param e Event received
		 */
	void onEvent(event::Event &e);

	/**
	 * @brief Adding a layer on top of the layers
	 * @param layer The new layer to add
	 */
	void pushLayer(shrd<layer::Layer> &&layer);
	/**
	* @brief Adding an overlay on top of everything
	* @param overlay The new overlay
	*/
	void pushOverlay(shrd<layer::Layer> &&overlay);

	/**
	 * @brief Access to the window
	 * @return The Window
	 */
	[[nodiscard]] const window::Window &getWindow() const {
		return *appWindow.get();
	}

	/**
	 * @brief Get the working directory
	 * @return The current working directory
	 */
	[[nodiscard]] const std::filesystem::path &getWorkingDirectory() const {
		return workingDirectory;
	}

#ifdef IMGUI_IMPL_HAS_DOCKING
	void enableDocking();
	void disableDocking();
#endif
private:
	/**
	* @brief Action on window close.
	* @param e The close event
	* @return True if succeeded
	*/
	bool onWindowClosed(event::WindowCloseEvent &e);
	/**
	* @brief Action on window resize.
	* @param e the resize event
	* @return True if succeeded
	*/
	bool onWindowResized(event::WindowResizeEvent &e);
	/// Pointer to the window
	uniq<window::Window> appWindow;
	/// Pointer to the GUI Layer
	shrd<gui::ImGuiLayer> imGuiLayer;
	/// Running state
	bool running = true;
	/// If Window minimized
	bool minimized = false;
	/// The stack of layers
	layer::LayerStack layerStack;
	/// The application Instance
	static Application *instance;
	/// Base Path to the working Directory
	std::filesystem::path workingDirectory;

	Timestep stepper;
};

/**
 * @brief Create an application (Must be defined in the client)
 * @return The application
 */
extern shrd<Application> createApplication();

}// namespace owl::core
