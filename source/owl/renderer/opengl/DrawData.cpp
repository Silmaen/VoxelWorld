/**
 * @file DrawData.cpp
 * @author Silmaen
 * @date 05/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "DrawData.h"
#include "renderer/Renderer.h"

namespace owl::renderer::opengl {

DrawData::~DrawData() = default;

void DrawData::init(const BufferLayout &layout, const std::string &renderer, std::vector<uint32_t> &indices, const std::string &shaderName) {
	if (layout.getStride() > 0) {
		vertexArray = mk_shared<opengl::VertexArray>();
		vertexBuffer = mk_shared<opengl::VertexBuffer>(layout.getStride() * indices.size());
		vertexBuffer->setLayout(layout);
		vertexArray->addVertexBuffer(vertexBuffer);
		vertexArray->setIndexBuffer(mk_shared<IndexBuffer>(indices.data(), indices.size()));
		setShader(shaderName, renderer);
	}
}

void DrawData::bind() const {
	if (shader)
		shader->bind();
	if (vertexArray)
		vertexArray->bind();
}

void DrawData::unbind() const {
	if (vertexArray)
		vertexArray->unbind();
	if (shader)
		shader->unbind();
}

void DrawData::setVertexData(const void *data, uint32_t size) {
	if (vertexBuffer)
		vertexBuffer->setData(data, size);
}

uint32_t DrawData::getIndexCount() const {
	if (vertexArray)
		return vertexArray->getIndexBuffer()->getCount();
	return 0;
}

void DrawData::setShader(const std::string &shaderName, const std::string &renderer) {
	auto &shLib = Renderer::getShaderLibrary();
	if (!shLib.exists(shaderName, renderer))
		shLib.addFromStandardPath(shaderName, renderer);
	shader = static_pointer_cast<Shader>(shLib.get(shaderName, renderer));
}

}// namespace owl::renderer::opengl
