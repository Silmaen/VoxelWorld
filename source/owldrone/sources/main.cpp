/**
 * @file main.cpp
 * @author Silmaen
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
// ------ Entry Point ------
#include "droneLayer.h"
#include <core/EntryPoint.h>
#include <debug/TrakerClient.h>
// -------------------------

class OwlNest : public owl::core::Application {
public:
	OwlNest() = delete;
	explicit OwlNest(const owl::core::AppParams &param) : owl::core::Application(param) {
		if (getState() == owl::core::Application::State::Running)
			pushLayer(owl::mk_shared<drone::droneLayer>());
	}
};

owl::shared<owl::core::Application> owl::core::createApplication(int argc, char **argv) {
	return mk_shared<OwlNest>(core::AppParams{
			.name = "Owl Drone - Navigator for drone",
#ifdef OWL_ASSETS_LOCATION
			.assetsPattern = OWL_ASSETS_LOCATION,
#endif
			.icon = "icons/logo_drone.png",
			.argCount = argc,
			.args = argv,
	});
}