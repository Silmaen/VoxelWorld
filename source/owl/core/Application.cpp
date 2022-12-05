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

Application::Application() {
  appWindow = window::Window::Create();
  appWindow->setEventCallback(
      [this](auto &&PH1) { onEvent(std::forward<decltype(PH1)>(PH1)); });
}

void Application::run() {
  while (running) {
    appWindow->onUpdate();
  }
}
void Application::onEvent(event::Event &e) {

  event::EventDispatcher dispatcher(e);
  dispatcher.dispatch<event::WindowCloseEvent>([this](auto &&PH1) {
    return onWindowClosed(std::forward<decltype(PH1)>(PH1));
  });
  OWL_CORE_TRACE(e.toString());
}

bool Application::onWindowClosed(event::WindowCloseEvent &) {
  running = false;
  return true;
}

} // namespace owl::core
