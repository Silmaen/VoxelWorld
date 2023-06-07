/**
 * @file UniformBuffer.cpp
 * @author Silmaen
 * @date 30/06/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "UniformBuffer.h"

#include "gl_21/glad.h"

namespace owl::renderer::opengl_legacy {

UniformBuffer::UniformBuffer([[maybe_unused]] uint32_t size, [[maybe_unused]] uint32_t binding) {
	gl_21::glGenBuffers(1, &rendererId);
	// todo: OpenGL 3.0
	//gl_21::glBindBuffer(GL_ARRAY_BUFFER, binding, rendererId);
	//gl_21::glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

UniformBuffer::~UniformBuffer() {
	gl_21::glDeleteBuffers(1, &rendererId);
}

void UniformBuffer::setData([[maybe_unused]] const void *data, [[maybe_unused]] uint32_t size, [[maybe_unused]] uint32_t offset) {
	//gl_21::glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

}// namespace owl::renderer::opengl_legacy
