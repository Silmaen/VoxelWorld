/**
 * @file UniformBuffer.cpp
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "UniformBindingLibrary.h"
#include "UniformBuffer.h"
#include "core/external/opengl21.h"

namespace owl::renderer::opengl_legacy {

UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding) {
	internalBinding = binding;
	internalData.resize(size);
	UniformBindingLibrary::get().addUniformBuffer(this);
}

UniformBuffer::~UniformBuffer() {
	UniformBindingLibrary::get().removeUniformBuffer(internalBinding);
}

void UniformBuffer::setData(const void *data, uint32_t size, uint32_t offset) {
	memcpy(internalData.data() + offset, data, size);
}

}// namespace owl::renderer::opengl_legacy
