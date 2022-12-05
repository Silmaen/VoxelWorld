/**
 * @file Application.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Application.h"
#include "Log.h"

namespace owl::core {

shrd<Application> Application::instance = nullptr;

Application::Application() {
  OWL_CORE_ASSERT(!instance, "Application already exists!");
  instance.reset(this);
  appWindow = window::Window::Create();
  appWindow->setEventCallback(
      [this](auto &&PH1) { onEvent(std::forward<decltype(PH1)>(PH1)); });
}

void Application::run() {
  while (running) {
    for (auto &layer : layerStack)
      layer->onUpdate();
    appWindow->onUpdate();
  }
}
void Application::onEvent(event::Event &e) {

  event::EventDispatcher dispatcher(e);
  dispatcher.dispatch<event::WindowCloseEvent>([this](auto &&PH1) {
    return onWindowClosed(std::forward<decltype(PH1)>(PH1));
  });

  for (auto &layer : layerStack) {
    layer->onEvent(e);
    if (e.handled)
      break;
  }
  if (!e.handled) {
    OWL_CORE_TRACE("Unhandled event: {}", e.toString());
  }
}

bool Application::onWindowClosed(event::WindowCloseEvent &) {
  running = false;
  return true;
}
void Application::pushLayer(shrd<layer::Layer> &&layer) {
  layerStack.pushLayer(std::move(layer));
}
void Application::pushOverlay(shrd<layer::Layer> &&overlay) {
  layerStack.pushOverlay(std::move(overlay));
}

} // namespace owl::core
