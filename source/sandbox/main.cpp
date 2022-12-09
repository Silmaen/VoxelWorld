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

#include "baseLayer.h"

class Sandbox : public owl::core::Application {
public:
	Sandbox() { pushLayer(owl::mk_shrd<baseLayer>()); }
};

owl::shrd<owl::core::Application> owl::core::createApplication() {
	return owl::mk_shrd<Sandbox>();
}
