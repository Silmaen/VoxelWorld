/**
 * @file VertexArray.cpp
 * @author Silmen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "VertexArray.h"

#include <glad/glad.h>

static GLenum ShaderDataTypeToOpenGLBaseType(owl::renderer::ShaderDataType type) {
	switch (type) {
		case owl::renderer::ShaderDataType::Float:
		case owl::renderer::ShaderDataType::Float2:
		case owl::renderer::ShaderDataType::Float3:
		case owl::renderer::ShaderDataType::Float4:
		case owl::renderer::ShaderDataType::Mat3:
		case owl::renderer::ShaderDataType::Mat4:
			return GL_FLOAT;
		case owl::renderer::ShaderDataType::Int:
		case owl::renderer::ShaderDataType::Int2:
		case owl::renderer::ShaderDataType::Int3:
		case owl::renderer::ShaderDataType::Int4:
			return GL_INT;
		case owl::renderer::ShaderDataType::Bool:
			return GL_BOOL;
		case owl::renderer::ShaderDataType::None:
			return GL_ZERO;
	}

	OWL_CORE_ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}

namespace owl::renderer::opengl {

VertexArray::VertexArray() {
	glCreateVertexArrays(1, &rendererID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::bind() const {
	glBindVertexArray(rendererID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(const VertexArray::vertexBuf &vertexBuffer) {
	OWL_CORE_ASSERT(!vertexBuffer->getLayout().getElements().empty(), "Vertex Buffer has no layout!");

	glBindVertexArray(rendererID);
	vertexBuffer->bind();

	uint32_t index = 0;
	const auto& layout = vertexBuffer->getLayout();
	for (const auto& element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index,
							  element.getComponentCount(),
							  ShaderDataTypeToOpenGLBaseType(element.type),
							  element.normalized ? GL_TRUE : GL_FALSE,
							  layout.getStride(),
							  reinterpret_cast<const void*>(element.offset));
		index++;
	}
	vertexBuffers.push_back(vertexBuffer);
}

void VertexArray::setIndexBuffer(const VertexArray::indexBuf &indexBuffer_) {
	glBindVertexArray(rendererID);
	indexBuffer->bind();

	indexBuffer = indexBuffer_;
}



}// namespace owl::renderer::opengl
