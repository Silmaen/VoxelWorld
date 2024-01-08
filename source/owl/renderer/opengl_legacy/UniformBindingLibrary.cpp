/**
 * @file UniformBindingLibrary.cpp
 * @author Silmaen
 * @date 01/09/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "UniformBindingLibrary.h"

namespace owl::renderer::opengl_legacy {


const UniformBuffer *UniformBindingLibrary::getUniformBuffer(uint32_t binding) {
	auto result = std::find_if(uniforms.begin(), uniforms.end(), [&binding](auto &item) { return item->getBinding() == binding; });
	if (result == uniforms.end())
		return nullptr;
	return *result;
}

void UniformBindingLibrary::addUniformBuffer(const UniformBuffer *buffer) {
	removeUniformBuffer(buffer->getBinding());
	uniforms.push_back(buffer);
}

void UniformBindingLibrary::removeUniformBuffer(uint32_t binding) {
	std::remove_if(uniforms.begin(), uniforms.end(), [&binding](auto &item) { return item->getBinding() == binding; });
}

}// namespace owl::renderer::opengl_legacy
