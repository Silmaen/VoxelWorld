/**
 * @file UniformBuffer.cpp
 * @author Silmaen
 * @date 02/01/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "UniformBuffer.h"

#include <glad/glad.h>

namespace owl::renderer::opengl {

UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding) {
	glCreateBuffers(1, &rendererId);
	glNamedBufferData(rendererId, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, rendererId);
}

UniformBuffer::~UniformBuffer() {
	glDeleteBuffers(1, &rendererId);
}

void UniformBuffer::setData(const void *data, uint32_t size, uint32_t offset) {
	glNamedBufferSubData(rendererId, offset, size, data);
}


}// namespace owl::renderer::opengl
