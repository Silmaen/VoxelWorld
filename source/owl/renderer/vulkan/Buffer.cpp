/**
 * @file Buffer.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Buffer.h"


namespace owl::renderer::vulkan {

VertexBuffer::VertexBuffer(uint32_t) {}

VertexBuffer::VertexBuffer(float *, uint32_t) {}

VertexBuffer::~VertexBuffer() {}

void VertexBuffer::bind() const {}

void VertexBuffer::unbind() const {}

void VertexBuffer::setData(const void *, uint32_t) {}

IndexBuffer::IndexBuffer(uint32_t *, uint32_t size) : count(size) {}

IndexBuffer::~IndexBuffer() {}

void IndexBuffer::bind() const {}

void IndexBuffer::unbind() const {}

}// namespace owl::renderer::vulkan
