/**
 * @file Buffer.cpp
 * @author Silmaen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Buffer.h"

#include "gl_46/glad.h"

namespace owl::renderer::opengl {

VertexBuffer::VertexBuffer(uint32_t size) {
	OWL_PROFILE_FUNCTION()

	gl_46::glCreateBuffers(1, &rendererID);
	gl_46::glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	gl_46::glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer(float *vertices, uint32_t size) {
	OWL_PROFILE_FUNCTION()

	gl_46::glCreateBuffers(1, &rendererID);
	gl_46::glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	gl_46::glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	OWL_PROFILE_FUNCTION()

	gl_46::glDeleteBuffers(1, &rendererID);
}

void VertexBuffer::bind() const {
	OWL_PROFILE_FUNCTION()

	gl_46::glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void VertexBuffer::unbind() const {
	OWL_PROFILE_FUNCTION()

	gl_46::glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(const void *data, uint32_t size) {
	gl_46::glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	gl_46::glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}


IndexBuffer::IndexBuffer(uint32_t *indices, uint32_t size) : count(size) {
	OWL_PROFILE_FUNCTION()

	gl_46::glCreateBuffers(1, &rendererID);
	gl_46::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	gl_46::glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	OWL_PROFILE_FUNCTION()

	gl_46::glDeleteBuffers(1, &rendererID);
}

void IndexBuffer::bind() const {
	OWL_PROFILE_FUNCTION()

	gl_46::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void IndexBuffer::unbind() const {
	OWL_PROFILE_FUNCTION()

	gl_46::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}// namespace owl::renderer::opengl
