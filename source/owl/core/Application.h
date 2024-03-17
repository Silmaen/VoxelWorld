/**
 * @file Application.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Timestep.h"
#include "event/AppEvent.h"
#include "gui/UiLayer.h"
#include "input/Window.h"
#include "layer/LayerStack.h"
#include "renderer/RenderAPI.h"

int main(int iArgc, char *iArgv[]);

namespace owl::core {
/**
 * @brief Parameters to give to the application.
 */
struct OWL_API AppParams {
	/// Application's title.
	std::string name = "Owl Engine";
	/// Application's assets pattern.
	std::string assetsPattern = "assets";
	/// Application's icon.
	std::string icon{};
	/// Windows width.
	uint32_t width = 1600;
	/// Windows height.
	uint32_t height = 960;
	/// Renderer's type.
	renderer::RenderAPI::Type renderer = renderer::RenderAPI::Type::OpenGL;
	/// If the application should use ImGui overlay.
	bool hasGui = true;
	/// Number of command line arguments.
	int argCount = 0;
	/// List of command line argument.
	char **args = nullptr;
	/// If extra debugging symbols should be loaded.
	bool useDebugging = false;
	/// The frequency for the frame debugging
	uint64_t frameLogFrequency = 0;

	/**
	 * @brief Access to the given command line argument.
	 * @param[in] iIndex Index of the argument.
	 * @return The argument.
	 */
	const char *operator[](const int iIndex) const {
		OWL_CORE_ASSERT(iIndex < argCount, "Bad command line index.")
		return args[iIndex];
	}

	/**
	 * @brief Load from a yaml config file.
	 * @param[in] iFile The file to load.
	 */
	void loadFromFile(const std::filesystem::path &iFile);

	/**
	 * @brief Save To a yaml file.
	 * @param[in] iFile The file to save.
	 */
	void saveToFile(const std::filesystem::path &iFile) const;
};

/**
 * @brief Root class defining the application to run.
 */
class OWL_API Application {
public:
	Application() = delete;

	Application(const Application &) = delete;

	Application(Application &&) = delete;

	Application &operator=(const Application &) = delete;

	Application &operator=(Application &&) = delete;

	/**
	 * @brief Default constructor.
	 * @param[in] iAppParams Application parameters.
	 */
	explicit Application(AppParams iAppParams);

	/**
	 * @brief Access to Application instance.
	 * @return Single instance of application.
	 */
	static Application &get() { return *s_instance; }

	/**
	 * @brief Destructor.
	 */
	virtual ~Application();

	/**
	 * @brief Event Callback function.
	 * @param[in,out] ioEvent Event received.
	 */
	void onEvent(event::Event &ioEvent);

	/**
	 * @brief Adding a layer on top of the layers.
	 * @param[in] iLayer The new layer to add.
	 */
	void pushLayer(shared<layer::Layer> &&iLayer);

	/**
	* @brief Adding an overlay on top of everything.
	* @param[in] iOverlay The new overlay.
	*/
	void pushOverlay(shared<layer::Layer> &&iOverlay);

	/**
	 * @brief Access to the window.
	 * @return The Window.
	 */
	[[nodiscard]] const input::Window &getWindow() const { return *mu_appWindow.get(); }

	/**
	 * @brief Access to the Gui layer.
	 * @return The gui layer.
	 */
	shared<gui::UiLayer> getImGuiLayer() { return mp_imGuiLayer; }

	/**
	 * @brief Request the application to terminate.
	 */
	void close();

	/**
	 * @brief Request the application to terminate.
	 */
	static void invalidate();

	/**
	 * @brief Get the working directory.
	 * @return The current working directory.
	 */
	[[nodiscard]] const std::filesystem::path &getWorkingDirectory() const { return m_workingDirectory; }

	/**
	 * @brief Get the working directory.
	 * @return The current working directory.
	 */
	[[nodiscard]] const std::filesystem::path &getAssetDirectory() const { return m_assetDirectory; }

	/**
	 * @brief Helper function used to redefine assets location.
	 * @param[in] iPattern The pattern to search for.
	 * @return True if found the assets.
	 */
	bool searchAssets(const std::string &iPattern);

	/**
		 * @brief Enable the docking environment.
		 */
	void enableDocking();

	/**
	 * @brief Disable the docking environment.
	 */
	void disableDocking();

	/**
	 * @brief Access to init parameters.
	 * @return Init parameters.
	 */
	[[nodiscard]] const AppParams &getInitParams() const { return m_initParams; }

	/**
	 * @brief State of the application.
	 */
	enum struct State {
		Running,/// Application is running.
		Stopped,/// Application Stopped.
		Error/// Application in error.
	};

	/**
	 * @brief Get the application's state.
	 * @return The current application's state.
	 */
	[[nodiscard]] const State &getState() const { return m_state; }


	/**
	 * @brief Get the application's time stepper.
	 * @return The application's time stepper.
	 */
	[[nodiscard]] const Timestep &getTimeStep() const { return m_stepper; }

private:
	/**
	 * @brief Runs the application.
	 */
	void run();

	/**
	* @brief Action on window close.
	* @param[in,out] ioEvent The close event.
	* @return True if succeeded.
	*/
	bool onWindowClosed(event::WindowCloseEvent &ioEvent);

	/**
	* @brief Action on window resize.
	* @param[in,out] iEvent the resize event.
	* @return True if succeeded.
	*/
	bool onWindowResized(const event::WindowResizeEvent &iEvent);

	/// Pointer to the window.
	uniq<input::Window> mu_appWindow;
	/// Pointer to the GUI Layer.
	shared<gui::UiLayer> mp_imGuiLayer = nullptr;
	/// Running state.
	State m_state = State::Running;
	/// If Window minimized.
	bool m_minimized = false;
	/// The stack of layers.
	layer::LayerStack m_layerStack;
	/// Base Path to the working Directory.
	std::filesystem::path m_workingDirectory;
	/// Base Path to the asset Directory.
	std::filesystem::path m_assetDirectory;
	/// Time steps management.
	Timestep m_stepper;
	/// Initialization parameters.
	AppParams m_initParams;
	/// The application Instance.
	static Application *s_instance;

	/// Mark the main entrypoint function as friend.
	friend int ::main(int iArgc, char **iArgv);
};

/**
 * @brief Create an application (Must be defined in the client).
 * @param[in] iArgc Number of arguments.
 * @param[in] iArgv List of argument.
 * @return The application.
 */
extern shared<Application> createApplication(int iArgc, char **iArgv);

}// namespace owl::core
