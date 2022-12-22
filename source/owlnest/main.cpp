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
	OwlNest() : core::Application({.name = "Owl Nest - Owl Engine Editor",
#ifdef OWL_ASSETS_LOCATION
								   .assetsPattern = OWL_ASSETS_LOCATION,
#endif
				}) {
		pushLayer(mk_shrd<EditorLayer>());
	}
};

shrd<core::Application> core::createApplication() {
	return mk_shrd<OwlNest>();
}

}// namespace owl
