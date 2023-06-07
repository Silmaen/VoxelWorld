/**
 * @file UniformBuffer.cpp
 * @author Silmaen
 * @date 02/01/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "UniformBuffer.h"

#include "gl_46/glad.h"

namespace owl::renderer::opengl {

UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding) {
	gl_46::glCreateBuffers(1, &rendererId);
	gl_46::glNamedBufferData(rendererId, size, nullptr, GL_DYNAMIC_DRAW);
	gl_46::glBindBufferBase(GL_UNIFORM_BUFFER, binding, rendererId);
}

UniformBuffer::~UniformBuffer() {
	gl_46::glDeleteBuffers(1, &rendererId);
}

void UniformBuffer::setData(const void *data, uint32_t size, uint32_t offset) {
	gl_46::glNamedBufferSubData(rendererId, offset, size, data);
}


}// namespace owl::renderer::opengl
