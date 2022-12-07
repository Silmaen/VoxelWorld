/**
 * @file Application.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Application.h"

// TEMPORARY
#include <GLFW/glfw3.h>

namespace owl::core {

Application *Application::instance = nullptr;

Application::Application() {
	OWL_CORE_ASSERT(!instance, "Application already exists!");
	instance = this;
	appWindow = window::Window::Create();
	appWindow->setEventCallback(
			[this](auto &&PH1) { onEvent(std::forward<decltype(PH1)>(PH1)); });
	imGuiLayer = mk_shrd<gui::ImGuiLayer>();
	pushOverlay(imGuiLayer);
}

void Application::run() {
	while (running) {
		if (!minimized) {
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			{
				for (auto &layer: layerStack)
					layer->onUpdate();
			}
			imGuiLayer->Begin();
			{
				for (auto &layer: layerStack)
					layer->onImGuiRender();
			}
			imGuiLayer->End();
		}
		appWindow->onUpdate();
	}
}

void Application::onEvent(event::Event &e) {

	event::EventDispatcher dispatcher(e);
	dispatcher.dispatch<event::WindowCloseEvent>([this](auto &&PH1) {
		return onWindowClosed(std::forward<decltype(PH1)>(PH1));
	});
	dispatcher.dispatch<event::WindowResizeEvent>([this](auto &&PH1) {
		return onWindowResized(std::forward<decltype(PH1)>(PH1));
	});

	for (auto &layer: layerStack) {
		if (e.handled)
			break;
		layer->onEvent(e);
	}
}

bool Application::onWindowClosed(event::WindowCloseEvent &) {
	running = false;
	return true;
}

bool Application::onWindowResized(event::WindowResizeEvent &e) {
	if (e.getWidth() == 0 || e.getHeight() == 0) {
		minimized = true;
		return false;
	}
	minimized = false;
	return false;
}

void Application::pushLayer(shrd<layer::Layer> &&layer) {
	layerStack.pushLayer(std::move(layer));
}
void Application::pushOverlay(shrd<layer::Layer> &&overlay) {
	layerStack.pushOverlay(std::move(overlay));
}

}// namespace owl::core
