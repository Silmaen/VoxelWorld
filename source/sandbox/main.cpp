/**
 * @file main.cpp
 * @author Silmaen
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owl.h"


class Sandbox : public owl::core::Application {
public:
  Sandbox() {
    pushOverlay(owl::mk_shrd<owl::gui::ImGuiLayer>());
  }
};

owl::shrd<owl::core::Application> owl::core::createApplication() {
  return owl::mk_shrd<Sandbox>();
}
