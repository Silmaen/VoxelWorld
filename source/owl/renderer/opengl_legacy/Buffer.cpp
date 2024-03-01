/**
 * @file Buffer.cpp
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Buffer.h"

#include "core/external/opengl21.h"

namespace owl::renderer::opengl_legacy {

VertexBuffer::VertexBuffer(const uint32_t iSize) {
	OWL_PROFILE_FUNCTION()

	glGenBuffers(1, &m_rendererId);
	bind();
	glBufferData(GL_ARRAY_BUFFER, iSize, nullptr, GL_DYNAMIC_DRAW);
	unbind();
}

VertexBuffer::VertexBuffer(const float *iVertices, const uint32_t iSize) {
	OWL_PROFILE_FUNCTION()

	glGenBuffers(1, &m_rendererId);
	bind();
	glBufferData(GL_ARRAY_BUFFER, iSize, iVertices, GL_STATIC_DRAW);
	unbind();
}

VertexBuffer::~VertexBuffer() {
	OWL_PROFILE_FUNCTION()

	glDeleteBuffers(1, &m_rendererId);
}

void VertexBuffer::bind() const {
	OWL_PROFILE_FUNCTION()

	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
}

void VertexBuffer::unbind() const {
	OWL_PROFILE_FUNCTION()

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(const void *iData, const uint32_t iSize) {
	bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, iSize, iData);
	unbind();
}


IndexBuffer::IndexBuffer(const uint32_t *iIndices, const uint32_t iCount) : m_count(iCount) {
	OWL_PROFILE_FUNCTION()

	glGenBuffers(1, &m_rendererId);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_count * sizeof(uint32_t)), iIndices,
				 GL_STATIC_DRAW);
	unbind();
}

IndexBuffer::~IndexBuffer() {
	OWL_PROFILE_FUNCTION()

	glDeleteBuffers(1, &m_rendererId);
}

void IndexBuffer::bind() const {
	OWL_PROFILE_FUNCTION()

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
}

void IndexBuffer::unbind() const {
	OWL_PROFILE_FUNCTION()

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}// namespace owl::renderer::opengl_legacy
