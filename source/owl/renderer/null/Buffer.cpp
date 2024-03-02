/**
 * @file Buffer.cpp
 * @author Silmaen
 * @date 30/07/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Buffer.h"

namespace owl::renderer::null {

VertexBuffer::VertexBuffer(uint32_t) {}

VertexBuffer::VertexBuffer(float *, uint32_t) {}

VertexBuffer::~VertexBuffer() = default;

void VertexBuffer::bind() const {}

void VertexBuffer::unbind() const {}

void VertexBuffer::setData(const void *, uint32_t) {}

IndexBuffer::IndexBuffer(uint32_t *, const uint32_t iCount) : m_count(iCount) {}

IndexBuffer::~IndexBuffer() = default;

void IndexBuffer::bind() const {}

void IndexBuffer::unbind() const {}

}// namespace owl::renderer::null
