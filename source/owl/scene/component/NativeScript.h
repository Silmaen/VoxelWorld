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
 * @brief Class NativeScript.
 */
struct OWL_API NativeScript {
	/// Script instance.
	ScriptableEntity* instance = nullptr;

	/**
	 * @brief Run the script.
	 * @return the script instance.
	 */
	ScriptableEntity*(*instantiateScript)(){};

	/**
	 * @brief Run destroy script.
	 */
	[[maybe_unused]] void (*destroyScript)(NativeScript*){};

	/**
	 * @brief Bind the script.
	 * @tparam T New script.
	 */
	template <typename T>
	void bind() {
		instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
		destroyScript = [](NativeScript* nsc) {
			delete nsc->instance;
			nsc->instance = nullptr;
		};
	}
};
} // namespace owl::scene::component
