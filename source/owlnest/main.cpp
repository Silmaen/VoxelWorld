/**
 * @file main.cpp
 * @author Silmaen
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
// ------ Entry Point ------
#include "core/EntryPoint.h"
#include "debug/TrakerClient.h"
// -------------------------
#include "EditorLayer.h"

namespace owl {

class OwlNest : public core::Application {
public:
	OwlNest() = delete;
	explicit OwlNest(core::AppParams param) : core::Application(param) {
		pushLayer(mk_shrd<EditorLayer>());
	}
};

shrd<core::Application> core::createApplication(int argc, char **argv) {
	return mk_shrd<OwlNest>(core::AppParams{
			.name = "Owl Nest - Owl Engine Editor",
#ifdef OWL_ASSETS_LOCATION
			.assetsPattern = OWL_ASSETS_LOCATION,
#endif
			.argCount = argc,
			.args = argv,
	});
}

}// namespace owl
