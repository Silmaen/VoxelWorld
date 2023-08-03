/**
 * @file VertexArray.cpp
 * @author Silmen
 * @date 30/07/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "VertexArray.h"

namespace owl::renderer::null {

VertexArray::VertexArray(){
		OWL_PROFILE_FUNCTION()}

VertexArray::~VertexArray() {
	OWL_PROFILE_FUNCTION()
}

void VertexArray::bind() const {
	OWL_PROFILE_FUNCTION()
}

void VertexArray::unbind() const {
	OWL_PROFILE_FUNCTION()
}

void VertexArray::addVertexBuffer(const VertexArray::vertexBuf &vertexBuffer) {
	OWL_PROFILE_FUNCTION()

	OWL_CORE_ASSERT(!vertexBuffer->getLayout().getElements().empty(), "Vertex Buffer has no layout!")

	vertexBuffer->bind();

	const auto &layout = vertexBuffer->getLayout();
	for (const auto &element: layout) {
		const auto count = static_cast<int32_t>(element.getComponentCount());
		switch (element.type) {
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool: {
				vertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4: {
				for (int32_t i = 0; i < count; i++) {
					vertexBufferIndex++;
				}
				break;
			}
			case ShaderDataType::None:
				OWL_CORE_ASSERT(false, "Unknown ShaderDataType!")
				break;
		}
	}
	vertexBuffers.push_back(vertexBuffer);
}

void VertexArray::setIndexBuffer(const VertexArray::indexBuf &indexBuffer_) {
	OWL_PROFILE_FUNCTION()
	indexBuffer_->bind();

	indexBuffer = indexBuffer_;
}


}// namespace owl::renderer::null
