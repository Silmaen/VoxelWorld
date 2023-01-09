/**
 * @file Application.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Application.h"

#include "renderer/Renderer.h"
#include <ranges>

namespace owl::core {


Application *Application::instance = nullptr;

Application::Application(const AppParams &appParams): initParams{appParams} {
	OWL_PROFILE_FUNCTION()

	OWL_CORE_ASSERT(!instance, "Application already exists!")
	instance = this;

	// Setup a working directory
	// Assuming present of a folder 'res' containing the data
	workingDirectory = absolute(std::filesystem::current_path());
	OWL_CORE_INFO("Working directory: {}", workingDirectory.string())
	[[maybe_unused]]bool assetFound = searchAssets(appParams.assetsPattern);
	OWL_CORE_ASSERT(assetFound, "Unable to find assets")

	// startup the renderer
	renderer::RenderCommand::create(renderer::RenderAPI::Type::OpenGL);

	// create main window
	appWindow = input::Window::create({.title = appParams.name});
	appWindow->setEventCallback(
			[this](auto &&PH1) { onEvent(std::forward<decltype(PH1)>(PH1)); });

	renderer::Renderer::init();

	// create the GUI layer
	imGuiLayer = mk_shrd<gui::ImGuiLayer>();
	pushOverlay(imGuiLayer);

	OWL_CORE_TRACE("Application creation done.")
}

void Application::enableDocking() {
	imGuiLayer->enableDocking();
}

void Application::disableDocking() {
	imGuiLayer->disableDocking();
}

Application::~Application() {
	OWL_PROFILE_FUNCTION()

	renderer::Renderer::shutdown();
}

void Application::close() {
	running = false;
}

void Application::run() {
	OWL_PROFILE_FUNCTION()

	while (running) {
		if (!minimized) {
			OWL_PROFILE_SCOPE("RunLoop")

			stepper.update();

			if (!minimized) {
				{
					OWL_PROFILE_SCOPE("LayerStack onUpdate")

					for (auto &layer: layerStack)
						layer->onUpdate(stepper);
				}
				imGuiLayer->begin();
				{
					OWL_PROFILE_SCOPE("LayerStack onImUpdate")

					for (auto &layer: layerStack)
						layer->onImGuiRender(stepper);
				}
				imGuiLayer->end();
			}
		}
		appWindow->onUpdate();
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

	running = false;
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

void Application::pushLayer(shrd<layer::Layer> &&layer) {
	OWL_PROFILE_FUNCTION()

	layerStack.pushLayer(std::move(layer));
}

void Application::pushOverlay(shrd<layer::Layer> &&overlay) {
	OWL_PROFILE_FUNCTION()

	layerStack.pushOverlay(std::move(overlay));
}

bool Application::searchAssets(const std::string &pattern) {
	OWL_PROFILE_FUNCTION()

	std::filesystem::path parent = workingDirectory;
	std::filesystem::path assets = parent / pattern;
	while (parent != parent.root_path()) {
		if (exists(assets)) {
			assetDirectory = assets;
			return true;
		}
		parent = parent.parent_path();
		assets = parent / pattern;
	}
	assetDirectory = workingDirectory;
	return false;
}

}// namespace owl::core
