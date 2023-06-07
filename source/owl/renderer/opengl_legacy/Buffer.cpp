/**
 * @file Buffer.cpp
 * @author Silmaen
 * @date 30/06/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Buffer.h"
#include "Manager.h"

namespace owl::renderer::opengl_legacy {

VertexBuffer::VertexBuffer(uint32_t size) {
	OWL_PROFILE_FUNCTION()

	data.resize(size);
	std::fill(data.begin(), data.end(), 0.f);
}

VertexBuffer::VertexBuffer(float *vertices, uint32_t size) {
	OWL_PROFILE_FUNCTION()
	data.resize(size);
	for (auto &v: data) {
		v = *vertices;
		++vertices;
	}
}

VertexBuffer::~VertexBuffer() {
	OWL_PROFILE_FUNCTION()

	data.clear();
}

void VertexBuffer::bind() const {
	OWL_PROFILE_FUNCTION()

	Manager::get().bindVertexBuffer(const_cast<VertexBuffer *>(this));
}

void VertexBuffer::unbind() const {
	OWL_PROFILE_FUNCTION()

	Manager::get().bindVertexBuffer(nullptr);
}

void VertexBuffer::setData(const void *dta, uint32_t size) {
	data.resize(size);
	const auto *p_data = static_cast<const float *>(dta);
	for (auto &v: data) {
		v = *p_data;
		++p_data;
	}
}

//

IndexBuffer::IndexBuffer(uint32_t *indices, uint32_t size) : count(size) {
	OWL_PROFILE_FUNCTION()
	data.resize(size);
	for (auto &v: data) {
		v = *indices;
		++indices;
	}
}

IndexBuffer::~IndexBuffer() {
	OWL_PROFILE_FUNCTION()

	data.clear();
}

void IndexBuffer::bind() const {
	OWL_PROFILE_FUNCTION()

	Manager::get().bindIndexBuffer(const_cast<IndexBuffer *>(this));
}

void IndexBuffer::unbind() const {
	OWL_PROFILE_FUNCTION()

	Manager::get().bindIndexBuffer(nullptr);
}

}// namespace owl::renderer::opengl_legacy
