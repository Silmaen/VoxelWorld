/**
 * @file VertexArray.cpp
 * @author Silmaen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "VertexArray.h"

#include "gl_46/glad.h"


namespace owl::renderer::opengl {

namespace utils {
static gl_46::GLenum toGLBaseType(owl::renderer::ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
			return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return GL_INT;
		case ShaderDataType::Bool:
			return GL_BOOL;
		case ShaderDataType::None:
			break;
	}

	OWL_CORE_ASSERT(false, "Unknown ShaderDataType!")
	return 0;
}
}// namespace utils

VertexArray::VertexArray() {
	OWL_PROFILE_FUNCTION()

	gl_46::glCreateVertexArrays(1, &rendererID);
}

VertexArray::~VertexArray() {
	OWL_PROFILE_FUNCTION()

	gl_46::glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::bind() const {
	OWL_PROFILE_FUNCTION()

	gl_46::glBindVertexArray(rendererID);
}

void VertexArray::unbind() const {
	OWL_PROFILE_FUNCTION()

	gl_46::glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(const VertexArray::vertexBuf &vertexBuffer) {
	OWL_PROFILE_FUNCTION()

	OWL_CORE_ASSERT(!vertexBuffer->getLayout().getElements().empty(), "Vertex Buffer has no layout!")

	gl_46::glBindVertexArray(rendererID);
	vertexBuffer->bind();

	const auto &layout = vertexBuffer->getLayout();
	for (const auto &element: layout) {
		const auto count = static_cast<int32_t>(element.getComponentCount());
		const auto type = utils::toGLBaseType(element.type);
		const auto stride = static_cast<int>(layout.getStride());
		switch (element.type) {
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4: {
				gl_46::glEnableVertexAttribArray(vertexBufferIndex);
				gl_46::glVertexAttribPointer(vertexBufferIndex,
											 count, type,
											 element.normalized ? GL_TRUE : GL_FALSE,
											 stride,
											 reinterpret_cast<const void *>(element.offset));
				vertexBufferIndex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool: {
				gl_46::glEnableVertexAttribArray(vertexBufferIndex);
				gl_46::glVertexAttribIPointer(vertexBufferIndex,
											  count, type, stride,
											  reinterpret_cast<const void *>(element.offset));
				vertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4: {
				for (int32_t i = 0; i < count; i++) {
					gl_46::glEnableVertexAttribArray(vertexBufferIndex);
					gl_46::glVertexAttribPointer(vertexBufferIndex,
												 count, type,
												 element.normalized ? GL_TRUE : GL_FALSE,
												 stride,
												 reinterpret_cast<const void *>(element.offset + sizeof(float) * static_cast<uint32_t>(count * i)));
					gl_46::glVertexAttribDivisor(vertexBufferIndex, 1);
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

	gl_46::glBindVertexArray(rendererID);
	indexBuffer_->bind();

	indexBuffer = indexBuffer_;
}


}// namespace owl::renderer::opengl
