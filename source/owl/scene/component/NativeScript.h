/**
 * @file NativeScript.h
 * @author Silmaen
 * @date 26/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../ScriptableEntity.h"

namespace owl::scene::component {

/**
 * @brief Class NaticeScript
 */
struct OWL_API NativeScript {
public:
	ScriptableEntity *instance = nullptr;

	ScriptableEntity* (*instantiateScript)();
	void (*destroyScript)(NativeScript*);

	template<typename T>
	void bind() {
		instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
		destroyScript = [](NativeScript* nsc) {delete nsc->instance; nsc->instance=nullptr; };
	}

};

}// namespace owl::scene::component
