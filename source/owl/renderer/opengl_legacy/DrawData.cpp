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

void DrawData::init(const BufferLayout &iLayout, [[maybe_unused]] const std::string &iRenderer,
					std::vector<uint32_t> &iIndices, const std::string &iShaderName) {
	OWL_PROFILE_FUNCTION()

	// Load the shader !
	setShader(iShaderName, iRenderer);
	if (!mp_shader)
		return;
	// Create the buffers
	mp_vertexBuffer = mkShared<opengl_legacy::VertexBuffer>(iLayout.getStride() * iIndices.size());
	mp_vertexBuffer->setLayout(iLayout);
	mp_indexBuffer = mkShared<IndexBuffer>(iIndices.data(), iIndices.size());
	// appy the layout
	//applyLayout();
	m_active = true;
}

void DrawData::bind() const {
	OWL_PROFILE_FUNCTION()

	if (!m_active)
		return;
	mp_shader->bind();
	mp_vertexBuffer->bind();
	mp_indexBuffer->bind();
	applyLayout();
}

void DrawData::unbind() const {
	OWL_PROFILE_FUNCTION()

	if (!m_active)
		return;
	unApplyLayout();
	mp_shader->unbind();
	mp_vertexBuffer->unbind();
	mp_indexBuffer->unbind();
}

void DrawData::setVertexData(const void *iData, const uint32_t iSize) {
	OWL_PROFILE_FUNCTION()

	if (!m_active)
		return;
	mp_vertexBuffer->setData(iData, iSize);
}

uint32_t DrawData::getIndexCount() const {
	if (!m_active)
		return 0;
	return mp_indexBuffer->getCount();
}

void DrawData::setShader(const std::string &iShaderName, const std::string &iRenderer) {
	OWL_PROFILE_FUNCTION()

	auto &shLib = Renderer::getShaderLibrary();
	if (!shLib.exists(iShaderName, iRenderer))
		shLib.addFromStandardPath(iShaderName, iRenderer);
	mp_shader = static_pointer_cast<Shader>(shLib.get(iShaderName, iRenderer));
}

void DrawData::applyLayout() const {
	OWL_PROFILE_FUNCTION()

	const uint32_t program = mp_shader->getBindId();
	for (const auto &layout = mp_vertexBuffer->getLayout(); const auto &element: layout) {
		if (element.type == ShaderDataType::None)
			continue;
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
	const uint32_t program = mp_shader->getBindId();
	for (const auto &layout = mp_vertexBuffer->getLayout(); const auto &element: layout) {
		if (element.type == ShaderDataType::None)
			continue;
		const auto location = static_cast<GLuint>(glGetAttribLocation(program, element.name.c_str()));
		glDisableVertexAttribArray(location);
	}
}

}// namespace owl::renderer::opengl_legacy
