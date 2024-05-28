/**
 * @file main.cpp
 * @author Silmaen
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include <owl.h>

#include <core/EntryPoint.h>

#include "droneLayer.h"

OWL_DIAG_PUSH
OWL_DIAG_DISABLE_CLANG("-Wweak-vtables")
class OwlNest final : public owl::core::Application {
public:
	OwlNest() = delete;
	explicit OwlNest(const owl::core::AppParams &param) : owl::core::Application(param) {
		if (getState() == owl::core::Application::State::Running)
			pushLayer(owl::mkShared<drone::droneLayer>());
	}
};
OWL_DIAG_POP

owl::shared<owl::core::Application> owl::core::createApplication(int argc, char **argv) {
	return mkShared<OwlNest>(core::AppParams{
			.args = argv,
			.name = "Owl Drone - Navigator for drone",
#ifdef OWL_ASSETS_LOCATION
			.assetsPattern = OWL_ASSETS_LOCATION,
#endif
			.icon = "icons/logo_drone.png",
			.argCount = argc,
	});
}
