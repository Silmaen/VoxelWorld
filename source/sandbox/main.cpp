/**
 * @file main.cpp
 * @author Silmaen
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owl.h"

class SandboxLayer : public owl::core::layer::Layer {
public:
  SandboxLayer() : Layer("SandBox") {}
  void onUpdate() override { OWL_INFO("SandboxLayer::Update"); }
  void onEvent(owl::event::Event &e) override {
    OWL_TRACE("SandboxLayerEvent {}", e.toString());
  }
};

class Sandbox : public owl::core::Application {
public:
  Sandbox() {
    pushLayer(owl::mk_shrd<SandboxLayer>());
    pushOverlay(owl::mk_shrd<owl::gui::ImGuiLayer>());
  }
};

owl::shrd<owl::core::Application> owl::core::createApplication() {
  return owl::mk_shrd<Sandbox>();
}
