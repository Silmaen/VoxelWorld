/**
 * @file DrawData.cpp
 * @author Silmaen
 * @date 05/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "DrawData.h"

#include "core/external/opengl21.h"
#include "renderer/Renderer.h"

namespace owl::renderer::opengl_legacy {

DrawData::~DrawData() = default;

void DrawData::init(const BufferLayout &layout, [[maybe_unused]] const std::string &renderer, std::vector<uint32_t> &indices, const std::string &shaderName) {
	OWL_PROFILE_FUNCTION()

	// Load the shader !
	setShader(shaderName, renderer);
	if (!shader) return;
	// Create the buffers
	vertexBuffer = mk_shared<opengl_legacy::VertexBuffer>(layout.getStride() * indices.size());
	vertexBuffer->setLayout(layout);
	indexBuffer = mk_shared<IndexBuffer>(indices.data(), indices.size());
	// appy the layout
	//applyLayout();
	active = true;
}

void DrawData::bind() const {
	OWL_PROFILE_FUNCTION()

	if (!active) return;
	shader->bind();
	vertexBuffer->bind();
	indexBuffer->bind();
	applyLayout();
}

void DrawData::unbind() const {
	OWL_PROFILE_FUNCTION()

	if (!active) return;
	unApplyLayout();
	shader->unbind();
	vertexBuffer->unbind();
	indexBuffer->unbind();
}

void DrawData::setVertexData(const void *data, uint32_t size) {
	OWL_PROFILE_FUNCTION()

	if (!active) return;
	vertexBuffer->setData(data, size);
}

uint32_t DrawData::getIndexCount() const {
	if (!active) return 0;
	return indexBuffer->getCount();
}

void DrawData::setShader(const std::string &shaderName, const std::string &renderer) {
	OWL_PROFILE_FUNCTION()

	auto &shLib = Renderer::getShaderLibrary();
	if (!shLib.exists(shaderName, renderer))
		shLib.addFromStandardPath(shaderName, renderer);
	shader = static_pointer_cast<Shader>(shLib.get(shaderName, renderer));
}

void DrawData::applyLayout() const {
	OWL_PROFILE_FUNCTION()

	const uint32_t program = shader->getBindId();
	const auto &layout = vertexBuffer->getLayout();
	for (const auto &element: layout) {
		if (element.type == ShaderDataType::None) continue;
		const auto location = static_cast<GLuint>(glGetAttribLocation(program, element.name.c_str()));
		const auto count = static_cast<GLint>(element.getComponentCount());
		const auto stride = static_cast<GLsizei>(layout.getStride());
		glEnableVertexAttribArray(location);
		switch (element.type) {
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
				glVertexAttribPointer(location,
									  count, GL_FLOAT,
									  element.normalized ? GL_TRUE : GL_FALSE,
									  stride,
									  reinterpret_cast<const void *>(element.offset));
				break;
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				glVertexAttribPointer(location,
									  count, GL_INT,
									  GL_FALSE,
									  stride,
									  reinterpret_cast<const void *>(element.offset));
				break;
			case ShaderDataType::Bool:
				glVertexAttribPointer(location,
									  count, GL_BOOL,
									  GL_FALSE,
									  stride,
									  reinterpret_cast<const void *>(element.offset));
				break;
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
				OWL_CORE_WARN("Attribute type matrix is not yet supported with legacy opengl renderer.")
				break;
			case ShaderDataType::None:
				OWL_CORE_ASSERT(false, "Unknown ShaderDataType!")
				break;
		}
	}
}

void DrawData::unApplyLayout() const {
	const uint32_t program = shader->getBindId();
	const auto &layout = vertexBuffer->getLayout();
	for (const auto &element: layout) {
		if (element.type == ShaderDataType::None) continue;
		const auto location = static_cast<GLuint>(glGetAttribLocation(program, element.name.c_str()));
		glDisableVertexAttribArray(location);
	}
}

}// namespace owl::renderer::opengl_legacy
