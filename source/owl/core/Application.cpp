/**
 * @file Application.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Application.h"

#include "debug/Tracker.h"
#include "input/Input.h"
#include "renderer/Renderer.h"

namespace owl::core {


Application *Application::instance = nullptr;

Application::Application(const AppParams &appParams) : initParams{appParams} {
	OWL_PROFILE_FUNCTION()

	OWL_CORE_ASSERT(!instance, "Application already exists!")
	instance = this;
	{
		OWL_SCOPE_TRACE
		// Setup a working directory
		// Assuming present of a folder 'res' containing the data
		workingDirectory = absolute(std::filesystem::current_path());
		OWL_CORE_INFO("Working directory: {}", workingDirectory.string())
		[[maybe_unused]] bool assetFound = searchAssets(appParams.assetsPattern);
		OWL_CORE_ASSERT(assetFound, "Unable to find assets")

		// startup the renderer
		renderer::RenderCommand::create(appParams.renderer);
		// check renderer creation
		if (renderer::RenderCommand::getState() != renderer::RenderAPI::State::Created) {
			OWL_CORE_ERROR("ERROR while Creating Renderer")
			state = State::Error;
			return;
		}

		// create main window
		appWindow = input::Window::create({.title = appParams.name});
		input::Input::init();
		OWL_CORE_INFO("Window Created.")
	}
	// initialize the renderer
	renderer::Renderer::init();
	{
		OWL_SCOPE_TRACE
		// check renderer initialization
		if (renderer::RenderCommand::getState() != renderer::RenderAPI::State::Ready) {
			OWL_CORE_ERROR("ERROR while Initializing Renderer")
			state = State::Error;
			return;
		}
		OWL_CORE_INFO("Renderer initiated.")
		appWindow->setEventCallback(
				[this](auto &&PH1) { onEvent(std::forward<decltype(PH1)>(PH1)); });

		// create the GUI layer
		imGuiLayer = mk_shared<gui::ImGuiLayer>();
		pushOverlay(imGuiLayer);
	}
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
}

void Application::close() {
	state = State::Stopped;
}


void Application::invalidate() {
	instance = nullptr;
}

void Application::run() {
	OWL_PROFILE_FUNCTION()

	uint64_t frameCount = 0;
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
				{
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

}// namespace owl::core
