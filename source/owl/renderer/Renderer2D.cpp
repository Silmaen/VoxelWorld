/**
 * @file Renderer2D.cpp
 * @author Silmaen
 * @date 18/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Renderer.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
#include <glm/gtc/matrix_transform.hpp>

namespace owl::renderer {

struct internalData{
	shrd<VertexArray> quadVertexArray;
	shrd<Texture2D> whiteTexture;
	shrd<Shader> shader;
};

static shrd<internalData> data;

void Renderer2D::init() {
	data = mk_shrd<internalData>();
	data->quadVertexArray = VertexArray::create();
	float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};
	owl::shrd<owl::renderer::VertexBuffer> squareVB = owl::renderer::VertexBuffer::create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({{"a_Position", owl::renderer::ShaderDataType::Float3},
						 {"a_TexCoord", owl::renderer::ShaderDataType::Float2}
	});
	data->quadVertexArray->addVertexBuffer(squareVB);
	uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
	data->quadVertexArray->setIndexBuffer(owl::renderer::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	auto &shLib = Renderer::getShaderLibrary();

	data->whiteTexture = Texture2D::create(1,1);
	uint32_t whiteTextureData = 0xffffffff;
	data->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

	shLib.addFromStandardPath("texture");
	data->shader = shLib.get("texture");
	data->shader->bind();
	data->shader->setInt("u_Texture", 0);
}

void Renderer2D::shutdown() {
	data.reset();
}

void Renderer2D::beginScene(const CameraOrtho &camera) {
	data->shader->bind();
	data->shader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
	data->shader->setMat4("u_Transform", glm::mat4(1.0f));
}

void Renderer2D::endScene() {
}

void Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
	drawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::drawQuad([[maybe_unused]]const glm::vec3 &position, [[maybe_unused]]const glm::vec2 &size, const glm::vec4 &color) {
	data->shader->setFloat4("u_Color", color);
	data->whiteTexture->bind();
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	data->shader->setMat4("u_Transform", transform);
	data->quadVertexArray->bind();
	RenderCommand::drawIndexed(data->quadVertexArray);
}

void Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const shrd<Texture> &texture) {
	drawQuad({ position.x, position.y, 0.0f }, size, texture);
}
void Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const shrd<Texture> &texture) {
	data->shader->setFloat4("u_Color", glm::vec4(1.0f));
	texture->bind();
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	data->shader->setMat4("u_Transform", transform);
	data->quadVertexArray->bind();
	RenderCommand::drawIndexed(data->quadVertexArray);
}


}// namespace owl::renderer
