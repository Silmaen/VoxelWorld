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
	vertexArray = mk_shared<opengl::VertexArray>();
	vertexBuffer = mk_shared<opengl::VertexBuffer>(layout.getStride() * indices.size());
	vertexBuffer->setLayout(layout);
	vertexArray->addVertexBuffer(vertexBuffer);
	vertexArray->setIndexBuffer(mk_shared<IndexBuffer>(indices.data(), indices.size()));
	setShader(shaderName, renderer);
}

void DrawData::bind() const {
	shader->bind();
	vertexArray->bind();
}

void DrawData::unbind() const {
	vertexArray->unbind();
	shader->unbind();
}

void DrawData::setVertexData(const void *data, uint32_t size) {
	vertexBuffer->setData(data, size);
}

uint32_t DrawData::getIndexCount() const {
	return vertexArray->getIndexBuffer()->getCount();
}

void DrawData::setShader(const std::string &shaderName, const std::string &renderer) {
	auto &shLib = Renderer::getShaderLibrary();
	if (!shLib.exists(shaderName, renderer))
		shLib.addFromStandardPath(shaderName, renderer);
	shader = static_pointer_cast<Shader>(shLib.get(shaderName, renderer));
}

}// namespace owl::renderer::opengl
