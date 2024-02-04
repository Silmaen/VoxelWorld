/**
 * @file Application.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Application.h"

#include <cstdlib>
#include <utility>

#include "core/external/yaml.h"
#include "debug/Tracker.h"
#include "input/Input.h"
#include "renderer/Renderer.h"

namespace owl::core {


Application *Application::instance = nullptr;

Application::Application(AppParams appParams) : initParams{std::move(appParams)} {
	OWL_PROFILE_FUNCTION()

	OWL_CORE_ASSERT(!instance, "Application already exists!")
	instance = this;

	// Look for things on the storages
	{
		// Set up a working directory
		// Assuming present of a folder 'res' containing the data
		workingDirectory = absolute(std::filesystem::current_path());
		OWL_CORE_INFO("Working directory: {}", workingDirectory.string())
#ifdef WIN32
		_putenv_s("VK_LAYER_PATH", workingDirectory.string().c_str());
#else
		setenv("VK_LAYER_PATH", workingDirectory.string().c_str(), 1);
#endif

		// load config file if any
		auto configPath = workingDirectory / "config.yml";
		if (exists(configPath))
			initParams.loadFromFile(configPath);
		// save config
		initParams.saveToFile(configPath);

		[[maybe_unused]] bool assetFound = searchAssets(initParams.assetsPattern);
		OWL_CORE_ASSERT(assetFound, "Unable to find assets")
	}

	// Create the renderer
	{
		renderer::RenderCommand::create(initParams.renderer);
		// check renderer creation
		if (renderer::RenderCommand::getState() != renderer::RenderAPI::State::Created) {
			OWL_CORE_ERROR("ERROR while Creating Renderer")
			state = State::Error;
			return;
		}
	}

	// create main window
	{
		appWindow = input::Window::create({
				.title = initParams.name,
				.iconPath = initParams.icon.empty() ? "" : (assetDirectory / initParams.icon).string(),
				.width = initParams.width,
				.height = initParams.height,
		});
		input::Input::init();
		OWL_CORE_INFO("Window Created.")
	}

	// initialize the renderer
	{
		renderer::Renderer::init();
		// check renderer initialization
		if (renderer::RenderCommand::getState() != renderer::RenderAPI::State::Ready) {
			OWL_CORE_ERROR("ERROR while Initializing Renderer")
			state = State::Error;
			return;
		}
		OWL_CORE_INFO("Renderer initiated.")
	}

	// set up the callbacks
	appWindow->setEventCallback(
			[this](auto &&PH1) { onEvent(std::forward<decltype(PH1)>(PH1)); });

	// create the GUI layer
	if (renderer::RenderCommand::getAPI() == renderer::RenderAPI::Type::OpenGL || renderer::RenderCommand::getAPI() == renderer::RenderAPI::Type::OpenglLegacy) {
		imGuiLayer = mk_shared<gui::UILayer>();
		pushOverlay(imGuiLayer);
		OWL_CORE_TRACE("GUI Layer created.")
	}

	OWL_CORE_TRACE("Application creation done.")
}

void Application::enableDocking() {
	if (imGuiLayer)
		imGuiLayer->enableDocking();
}

void Application::disableDocking() {
	if (imGuiLayer)
		imGuiLayer->disableDocking();
}

Application::~Application() {
	OWL_PROFILE_FUNCTION()

	if (renderer::RenderAPI::getState() != renderer::RenderAPI::State::Error) {
		for (auto &layer: layerStack) {
			layerStack.popLayer(layer);
			layerStack.popOverlay(layer);
		}
		input::Input::invalidate();
		renderer::Renderer::shutdown();
		renderer::RenderCommand::invalidate();
		appWindow.reset();
	}
	invalidate();
}

void Application::close() {
	state = State::Stopped;
}

void Application::invalidate() {
	instance = nullptr;
}

void Application::run() {
	OWL_PROFILE_FUNCTION()

#if OWL_TRACKER_VERBOSITY >= 3
	uint64_t frameCount = 0;
#endif
	while (state == State::Running) {
		if (!minimized) {
			OWL_PROFILE_SCOPE("RunLoop")

			stepper.update();

			if (!minimized) {
				{
					OWL_PROFILE_SCOPE("LayerStack onUpdate")

					for (auto &layer: layerStack)
						layer->onUpdate(stepper);
				}
				if (imGuiLayer) {
					imGuiLayer->begin();
					{
						OWL_PROFILE_SCOPE("LayerStack onImUpdate")

						for (auto &layer: layerStack)
							layer->onImGuiRender(stepper);
					}
					imGuiLayer->end();
				}
			}
		}
		appWindow->onUpdate();

#if OWL_TRACKER_VERBOSITY >= 3
		{
			const auto &memState = debug::Tracker::get().checkState();
			if (memState.allocationCalls > memState.deallocationCalls && frameCount > 0) {
				OWL_CORE_TRACE("----------------------------------")
				OWL_CORE_TRACE("Frame Leak Detected")
				OWL_CORE_TRACE("-----------------------------------")
				OWL_CORE_TRACE("")
				OWL_CORE_TRACE(" LEAK Amount: {} in {} Unallocated chunks", memState.allocatedMemory, memState.allocs.size())
				for (const auto &chunk: memState.allocs) {
					OWL_CORE_TRACE(" ** {}", chunk.toStr())
				}
				OWL_CORE_TRACE("----------------------------------")
				OWL_CORE_TRACE("")
			}
		}
		++frameCount;
#endif
	}
}

void Application::onEvent(event::Event &e) {
	OWL_PROFILE_FUNCTION()

	event::EventDispatcher dispatcher(e);
	dispatcher.dispatch<event::WindowCloseEvent>([this](auto &&PH1) {
		return onWindowClosed(std::forward<decltype(PH1)>(PH1));
	});
	dispatcher.dispatch<event::WindowResizeEvent>([this](auto &&PH1) {
		return onWindowResized(std::forward<decltype(PH1)>(PH1));
	});

	//for (auto &it: std::ranges::reverse_view(layerStack)) {
	for (auto it = layerStack.rbegin(); it != layerStack.rend(); ++it) {
		if (e.handled)
			break;
		(*it)->onEvent(e);
	}
}

bool Application::onWindowClosed(event::WindowCloseEvent &) {
	OWL_PROFILE_FUNCTION()

	close();
	return true;
}

bool Application::onWindowResized(event::WindowResizeEvent &e) {
	OWL_PROFILE_FUNCTION()

	if (e.getWidth() == 0 || e.getHeight() == 0) {
		minimized = true;
		return false;
	}
	minimized = false;
	renderer::Renderer::onWindowResized(e.getWidth(), e.getHeight());
	return false;
}

void Application::pushLayer(shared<layer::Layer> &&layer) {
	OWL_PROFILE_FUNCTION()
	if (renderer::RenderCommand::getState() == renderer::RenderAPI::State::Error) return;
	layerStack.pushLayer(std::move(layer));
}

void Application::pushOverlay(shared<layer::Layer> &&overlay) {
	OWL_PROFILE_FUNCTION()
	if (renderer::RenderCommand::getState() == renderer::RenderAPI::State::Error) return;
	layerStack.pushOverlay(std::move(overlay));
}

bool Application::searchAssets(const std::string &pattern) {
	OWL_PROFILE_FUNCTION()

	std::filesystem::path parent = workingDirectory;
	std::filesystem::path assets = parent / pattern;
	while (parent != parent.root_path()) {
		if (exists(assets)) {
			assetDirectory = std::move(assets);
			return true;
		}
		assets.clear();
		parent = parent.parent_path();
		assets = parent;
		assets /= pattern;
	}
	assetDirectory = workingDirectory;
	return false;
}

void AppParams::loadFromFile(const std::filesystem::path &file) {
	YAML::Node data = YAML::LoadFile(file.string());
	auto appConfig = data["AppConfig"];
	if (appConfig) {
		if (appConfig["width"]) width = appConfig["width"].as<uint32_t>();
		if (appConfig["height"]) height = appConfig["height"].as<uint32_t>();
		if (appConfig["renderer"]) {
			auto dRenderer = magic_enum::enum_cast<renderer::RenderAPI::Type>(appConfig["renderer"].as<std::string>());
			if (dRenderer.has_value())
				renderer = dRenderer.value();
		}
	}
}

void AppParams::saveToFile(const std::filesystem::path &file) const {
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "AppConfig" << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "width" << YAML::Value << width;
	out << YAML::Key << "height" << YAML::Value << height;
	out << YAML::Key << "renderer" << YAML::Value << std::string(magic_enum::enum_name(renderer));

	out << YAML::EndMap;
	out << YAML::EndMap;
	std::ofstream fileOut(file);
	fileOut << out.c_str();
	fileOut.close();
}


}// namespace owl::core
