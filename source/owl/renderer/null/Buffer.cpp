/**
 * @file Buffer.cpp
 * @author Silmen
 * @date 30/07/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Buffer.h"


namespace owl::renderer::null {

VertexBuffer::VertexBuffer(uint32_t){
		OWL_PROFILE_FUNCTION()}

VertexBuffer::VertexBuffer(float *, uint32_t){
		OWL_PROFILE_FUNCTION()}

VertexBuffer::~VertexBuffer() {
	OWL_PROFILE_FUNCTION()
}

void VertexBuffer::bind() const {
	OWL_PROFILE_FUNCTION()
}

void VertexBuffer::unbind() const {
	OWL_PROFILE_FUNCTION()
}

void VertexBuffer::setData(const void *, uint32_t) {
}


IndexBuffer::IndexBuffer(uint32_t *, uint32_t size) : count(size){
															  OWL_PROFILE_FUNCTION()}

													  IndexBuffer::~IndexBuffer() {
	OWL_PROFILE_FUNCTION()
}

void IndexBuffer::bind() const {
	OWL_PROFILE_FUNCTION()
}

void IndexBuffer::unbind() const {
	OWL_PROFILE_FUNCTION()
}

}// namespace owl::renderer::null
