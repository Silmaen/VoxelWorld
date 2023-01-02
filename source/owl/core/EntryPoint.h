/**
 * @file EntryPoint.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once
#include "Application.h"
#include "Log.h"
#include "debug/Profiler.h"

int main(int argc, char *argv[]) {

	owl::core::Log::init();
	// Startup
	OWL_PROFILE_BEGIN_SESSION("Startup","OwlProfile-startup.json")
	auto app = owl::core::createApplication(argc, argv);
	OWL_PROFILE_END_SESSION()
	// runtime
	OWL_PROFILE_BEGIN_SESSION("Runtime","OwlProfile-runtime.json")
	app->run();
	OWL_PROFILE_END_SESSION()
	// Shutdown
	OWL_PROFILE_BEGIN_SESSION("Shutdown","OwlProfile-shutdown.json")
	app.reset();
	OWL_PROFILE_END_SESSION()
	return 0;
}
