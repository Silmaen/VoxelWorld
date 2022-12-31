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

/**
 * @brief Structure holding vertex information
 */
struct QuadVertex {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoord;
	float texIndex;
	float tilingFactor;
};

/**
 * @brief Structure holding static internal data
 */
struct internalData {
	static const uint32_t maxQuads = 20000;
	static const uint32_t maxVertices = maxQuads * 4;
	static const uint32_t maxIndices = maxQuads * 6;
	static const uint32_t maxTextureSlots = 32;// TODO: RenderCaps
	shrd<VertexArray> quadVertexArray;
	shrd<VertexBuffer> quadVertexBuffer;
	shrd<Texture2D> whiteTexture;
	shrd<Shader> shader;
	uint32_t quadIndexCount = 0;
	QuadVertex *quadVertexBufferBase = nullptr;
	QuadVertex *quadVertexBufferPtr = nullptr;

	std::array<shrd<Texture2D>, maxTextureSlots> textureSlots;
	uint32_t textureSlotIndex = 1;// 0 = white texture

	Renderer2D::Statistics stats;
};

static internalData data;

void Renderer2D::init() {
	OWL_PROFILE_FUNCTION()

	data.quadVertexArray = VertexArray::create();
	data.quadVertexBuffer = VertexBuffer::create(data.maxVertices * sizeof(QuadVertex));
	data.quadVertexBuffer->SetLayout({{"a_Position", ShaderDataType::Float3},
									  {"a_Color", ShaderDataType::Float4},
									  {"a_TexCoord", ShaderDataType::Float2},
									  {"a_TexIndex", ShaderDataType::Float},
									  {"a_TilingFactor", ShaderDataType::Float}});
	data.quadVertexArray->addVertexBuffer(data.quadVertexBuffer);

	data.quadVertexBufferBase = new QuadVertex[data.maxVertices];

	auto *quadIndices = new uint32_t[data.maxIndices];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < data.maxIndices; i += 6) {
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}
	data.quadVertexArray->setIndexBuffer(owl::renderer::IndexBuffer::create(quadIndices, data.maxIndices));
	delete quadIndices;

	data.whiteTexture = Texture2D::create(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

	int32_t samplers[data.maxTextureSlots];
	for (uint32_t i = 0; i < data.maxTextureSlots; ++i)
		samplers[i] = i;

	auto &shLib = Renderer::getShaderLibrary();
	shLib.addFromStandardPath("texture");
	data.shader = shLib.get("texture");
	data.shader->bind();
	data.shader->setIntArray("u_Textures", samplers, data.maxTextureSlots);

	// Set all texture slots to 0
	data.textureSlots[0] = data.whiteTexture;
}

void Renderer2D::shutdown() {
	OWL_PROFILE_FUNCTION()

	delete[] data.quadVertexBufferBase;
}

void Renderer2D::beginScene(const CameraOrtho &camera) {
	OWL_PROFILE_FUNCTION()

	data.shader->bind();
	data.shader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
	startBatch();
}

void Renderer2D::beginScene(const CameraEditor& camera){
	OWL_PROFILE_FUNCTION();

	glm::mat4 viewProj = camera.getViewProjection();

	data.shader->bind();
	data.shader->setMat4("u_ViewProjection", viewProj);
	startBatch();
}

void Renderer2D::beginScene(const Camera &camera, const glm::mat4 &transform) {
	OWL_PROFILE_FUNCTION()

	glm::mat4 viewProj = camera.getProjection() * glm::inverse(transform);

	data.shader->bind();
	data.shader->setMat4("u_ViewProjection", viewProj);
	startBatch();
}

void Renderer2D::endScene() {
	OWL_PROFILE_FUNCTION()

	flush();
}

void Renderer2D::flush() {
	if (data.quadIndexCount == 0)
		return;// Nothing to draw{
	auto dataSize = (uint32_t) ((uint8_t *) data.quadVertexBufferPtr - (uint8_t *) data.quadVertexBufferBase);
	data.quadVertexBuffer->setData(data.quadVertexBufferBase, dataSize);
	for (uint32_t i = 0; i < data.textureSlotIndex; i++)
		data.textureSlots[i]->bind(i);
	RenderCommand::drawIndexed(data.quadVertexArray, data.quadIndexCount);
	data.stats.drawCalls++;
}

void Renderer2D::startBatch() {
	data.quadIndexCount = 0;
	data.quadVertexBufferPtr = data.quadVertexBufferBase;

	data.textureSlotIndex = 1;
}

void Renderer2D::nextBatch() {
	flush();
	startBatch();
}
void Renderer2D::drawQuad(const Quad2DDataT &quadData) {
	OWL_PROFILE_FUNCTION()
	if (data.quadIndexCount >= internalData::maxIndices)
		nextBatch();
	float textureIndex = 0.0f;
	constexpr size_t quadVertexCount = 4;
	constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
	constexpr glm::vec4 quadVertexPosition[] = {{-0.5f, -0.5f, 0.0f, 1.0f},
												{0.5f, -0.5f, 0.0f, 1.0f},
												{0.5f, 0.5f, 0.0f, 1.0f},
												{-0.5f, 0.5f, 0.0f, 1.0f}};
	if (quadData.texture != nullptr) {
		for (uint32_t i = 1; i < data.textureSlotIndex; i++) {
			if (*data.textureSlots[i] == *quadData.texture) {
				textureIndex = (float) i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			if (data.textureSlotIndex >= internalData::maxTextureSlots)
				nextBatch();
			textureIndex = (float) data.textureSlotIndex;
			data.textureSlots[data.textureSlotIndex] = std::static_pointer_cast<Texture2D>(quadData.texture);
			data.textureSlotIndex++;
		}
	}
	for (size_t i = 0; i < quadVertexCount; i++) {
		data.quadVertexBufferPtr->position = quadData.transform * quadVertexPosition[i];
		data.quadVertexBufferPtr->color = quadData.color;
		data.quadVertexBufferPtr->texCoord = textureCoords[i];
		data.quadVertexBufferPtr->texIndex = textureIndex;
		data.quadVertexBufferPtr->tilingFactor = quadData.tilingFactor;
		data.quadVertexBufferPtr++;
	}
	data.quadIndexCount += 6;

	data.stats.quadCount++;
}

void Renderer2D::drawQuad(const Quad2DData &quadData) {
	OWL_PROFILE_FUNCTION()

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), quadData.position) *
						  glm::rotate(glm::mat4(1.0f), glm::radians(quadData.rotation), {0.0f, 0.0f, 1.0f}) *
						  glm::scale(glm::mat4(1.0f), {quadData.size.x, quadData.size.y, 1.0f});
	drawQuad({.transform = transform,
			  .color = quadData.color,
			  .texture = quadData.texture,
			  .tilingFactor = quadData.tilingFactor});
}

void Renderer2D::resetStats() {
	data.stats.drawCalls = 0;
	data.stats.quadCount = 0;
}

Renderer2D::Statistics Renderer2D::getStats() {
	return data.stats;
}

}// namespace owl::renderer
