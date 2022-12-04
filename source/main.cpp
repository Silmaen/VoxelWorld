/**
 * @file main.cpp
 * @author damien.lachouette
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "core/EngineGLFW.h"
#include <memory>

int main() {
  auto engine = std::make_shared<owl::core::EngineGLFW>();
  return engine->run();
}
