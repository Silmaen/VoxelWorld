/**
 * @file Buffer.cpp
 * @author Silmaen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Buffer.h"
#include "core/external/opengl46.h"

namespace owl::renderer::opengl {

VertexBuffer::VertexBuffer(uint32_t size) {
	OWL_PROFILE_FUNCTION()

	glCreateBuffers(1, &rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer(float *vertices, uint32_t size) {
	OWL_PROFILE_FUNCTION()

	glCreateBuffers(1, &rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	OWL_PROFILE_FUNCTION()

	glDeleteBuffers(1, &rendererID);
}

void VertexBuffer::bind() const {
	OWL_PROFILE_FUNCTION()

	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void VertexBuffer::unbind() const {
	OWL_PROFILE_FUNCTION()

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(const void *data, uint32_t size) {
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}


IndexBuffer::IndexBuffer(uint32_t *indices, uint32_t size) : count(size) {
	OWL_PROFILE_FUNCTION()

	glCreateBuffers(1, &rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	OWL_PROFILE_FUNCTION()

	glDeleteBuffers(1, &rendererID);
}

void IndexBuffer::bind() const {
	OWL_PROFILE_FUNCTION()

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void IndexBuffer::unbind() const {
	OWL_PROFILE_FUNCTION()

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}// namespace owl::renderer::opengl
