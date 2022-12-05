/**
 * @file EntryPoint.h
 * @author argawaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once
#include "Application.h"
#include "Log.h"

int main([[maybe_unused]]int argc, [[maybe_unused]] char* argv[]){
  owl::core::Log::init();
  OWL_CORE_WARN("Logger Initialize");
  OWL_INFO("Logger Initialize");

  auto app = owl::core::createApplication();
  app->run();
  app.reset();
  return 0;
}
