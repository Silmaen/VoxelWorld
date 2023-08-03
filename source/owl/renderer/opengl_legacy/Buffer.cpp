/**
 * @file Buffer.cpp
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Buffer.h"
#include "glad21/glad.h"

namespace owl::renderer::opengl_legacy {

VertexBuffer::VertexBuffer(uint32_t size) {
	OWL_PROFILE_FUNCTION()

	glGenBuffers(1, &rendererId);
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	unbind();
}

VertexBuffer::VertexBuffer(float *vertices, uint32_t size) {
	OWL_PROFILE_FUNCTION()

	glGenBuffers(1, &rendererId);
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	unbind();
}

VertexBuffer::~VertexBuffer() {
	OWL_PROFILE_FUNCTION()

	glDeleteBuffers(1, &rendererId);
}

void VertexBuffer::bind() const {
	OWL_PROFILE_FUNCTION()

	glBindBuffer(GL_ARRAY_BUFFER, rendererId);
}

void VertexBuffer::unbind() const {
	OWL_PROFILE_FUNCTION()

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(const void *data, uint32_t size) {
	bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	unbind();
}


IndexBuffer::IndexBuffer(uint32_t *indices, uint32_t size) : count(size) {
	OWL_PROFILE_FUNCTION()

	glGenBuffers(1, &rendererId);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	unbind();
}

IndexBuffer::~IndexBuffer() {
	OWL_PROFILE_FUNCTION()

	glDeleteBuffers(1, &rendererId);
}

void IndexBuffer::bind() const {
	OWL_PROFILE_FUNCTION()

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
}

void IndexBuffer::unbind() const {
	OWL_PROFILE_FUNCTION()

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}// namespace owl::renderer::opengl_legacy
