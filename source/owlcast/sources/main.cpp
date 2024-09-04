/**
 * @file main.cpp
 * @author Silmaen
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include <owl.h>

#include <core/EntryPoint.h>

#include "EditorLayer.h"

namespace owl {

OWL_DIAG_PUSH
OWL_DIAG_DISABLE_CLANG("-Wweak-vtables")
class OwlCast final : public core::Application {
public:
	OwlCast() = delete;
	explicit OwlCast(const core::AppParams& param) : core::Application(param) {
		if (getState() == core::Application::State::Running)
			pushLayer(mkShared<raycaster::EditorLayer>());
	}
};
OWL_DIAG_POP

auto core::createApplication(int argc, char** argv) -> shared<core::Application> {
	return mkShared<OwlCast>(core::AppParams{
			.args = argv,
			.name = "Owl Caster",
#ifdef OWL_ASSETS_LOCATION
			.assetsPattern = OWL_ASSETS_LOCATION,
#endif
			.icon = "icons/logo_owl_icon.png",
			.argCount = argc,
	});
}

}// namespace owl
