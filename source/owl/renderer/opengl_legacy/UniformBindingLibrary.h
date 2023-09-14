/**
* @file UniformBindingLibrary.h
* @author Silmaen
* @date 01/09/2023
* Copyright © 2023 All rights reserved.
* All modification must get authorization from the author.
*/
#pragma once

#include "UniformBuffer.h"

namespace owl::renderer::opengl_legacy {

/**
 * @brief Class holding the uniforms as binding.
 */
class UniformBindingLibrary {
public:
	static UniformBindingLibrary &get() {
		static UniformBindingLibrary instance;
		return instance;
	}

	const UniformBuffer *getUniformBuffer(uint32_t binding);

	void addUniformBuffer(const UniformBuffer *buffer);

	void removeUniformBuffer(uint32_t binding);

private:
	UniformBindingLibrary() = default;

	std::vector<const UniformBuffer *> uniforms;
};

}// namespace owl::renderer::opengl_legacy
