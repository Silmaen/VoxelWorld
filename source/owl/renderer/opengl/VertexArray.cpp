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

	OWL_CORE_ASSERT(false, "Unknown ShaderDataType!")
	return 0;
}

namespace owl::renderer::opengl {

VertexArray::VertexArray() {
	OWL_PROFILE_FUNCTION()

	glCreateVertexArrays(1, &rendererID);
}

VertexArray::~VertexArray() {
	OWL_PROFILE_FUNCTION()

	glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::bind() const {
	OWL_PROFILE_FUNCTION()

	glBindVertexArray(rendererID);
}

void VertexArray::unbind() const {
	OWL_PROFILE_FUNCTION()

	glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(const VertexArray::vertexBuf &vertexBuffer) {
	OWL_PROFILE_FUNCTION()

	OWL_CORE_ASSERT(!vertexBuffer->getLayout().getElements().empty(), "Vertex Buffer has no layout!")

	glBindVertexArray(rendererID);
	vertexBuffer->bind();

	const auto &layout = vertexBuffer->getLayout();
	for (const auto &element: layout) {
		switch (element.type) {
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:{
				glEnableVertexAttribArray(VBIndexOffset);
				glVertexAttribPointer(VBIndexOffset,
									  element.getComponentCount(),
									  ShaderDataTypeToOpenGLBaseType(element.type),
									  element.normalized ? GL_TRUE : GL_FALSE,
									  layout.getStride(),
									  reinterpret_cast<const void *>(element.offset));
				VBIndexOffset++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool: {
				glEnableVertexAttribArray(VBIndexOffset);
				glVertexAttribIPointer(VBIndexOffset,
									  element.getComponentCount(),
									  ShaderDataTypeToOpenGLBaseType(element.type),
									  layout.getStride(),
									  reinterpret_cast<const void *>(element.offset));
				VBIndexOffset++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4: {
				uint8_t count = element.getComponentCount();
				for (uint8_t i = 0; i < count; i++) {
					glEnableVertexAttribArray(VBIndexOffset);
					glVertexAttribPointer(VBIndexOffset,
										  count,
										  ShaderDataTypeToOpenGLBaseType(element.type),
										  element.normalized ? GL_TRUE : GL_FALSE,
										  layout.getStride(),
										  reinterpret_cast<const void *>(element.offset + sizeof(float) * count * i));
							glVertexAttribDivisor(VBIndexOffset, 1);
							VBIndexOffset++;
				}
				break;
			}
			default:
				OWL_CORE_ASSERT(false, "Unknown ShaderDataType!");
		}
	}
	vertexBuffers.push_back(vertexBuffer);
}

void VertexArray::setIndexBuffer(const VertexArray::indexBuf &indexBuffer_) {
	OWL_PROFILE_FUNCTION()

	glBindVertexArray(rendererID);
	indexBuffer_->bind();

	indexBuffer = indexBuffer_;
}


}// namespace owl::renderer::opengl
