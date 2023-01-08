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

namespace utils {
/**
 * @brief Structure holding vertex information
 */
struct QuadVertex {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoord;
	float texIndex;
	float tilingFactor;
	int entityID;
};

constexpr uint32_t maxQuads = 20000;
constexpr size_t quadVertexCount = 4;
constexpr uint32_t maxVertices = maxQuads * quadVertexCount;
constexpr uint32_t maxIndices = maxQuads * 6;
constexpr int32_t maxTextureSlots = 32;// TODO: RenderCaps
constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
constexpr glm::vec4 quadVertexPosition[] = {{-0.5f, -0.5f, 0.0f, 1.0f},
											{0.5f, -0.5f, 0.0f, 1.0f},
											{0.5f, 0.5f, 0.0f, 1.0f},
											{-0.5f, 0.5f, 0.0f, 1.0f}};
/**
 * @brief Structure holding static internal data
 */
struct internalData {
	shrd<VertexArray> quadVertexArray;
	shrd<VertexBuffer> quadVertexBuffer;
	shrd<Texture2D> whiteTexture;
	shrd<Shader> shader;
	uint32_t quadIndexCount = 0;
	std::vector<QuadVertex> quadVertexBuf;
	std::array<shrd<Texture2D>, maxTextureSlots> textureSlots;
	uint32_t textureSlotIndex = 1;// 0 = white texture
	Renderer2D::Statistics stats;
};

}

static utils::internalData data;

void Renderer2D::init() {
	OWL_PROFILE_FUNCTION()

	data.quadVertexArray = VertexArray::create();
	data.quadVertexBuffer = VertexBuffer::create(utils::maxVertices * sizeof(utils::QuadVertex));
	data.quadVertexBuffer->setLayout({
			{"a_Position", ShaderDataType::Float3},
			{"a_Color", ShaderDataType::Float4},
			{"a_TexCoord", ShaderDataType::Float2},
			{"a_TexIndex", ShaderDataType::Float},
			{"a_TilingFactor", ShaderDataType::Float},
			{"a_EntityID", ShaderDataType::Int},
	});
	data.quadVertexArray->addVertexBuffer(data.quadVertexBuffer);

	data.quadVertexBuf.reserve(utils::maxVertices);

	{
		std::vector<uint32_t> quadIndices;
		quadIndices.resize(utils::maxIndices);
		uint32_t offset = 0;
		for (uint32_t i = 0; i < utils::maxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		data.quadVertexArray->setIndexBuffer(owl::renderer::IndexBuffer::create(quadIndices.data(), utils::maxIndices));
	}

	data.whiteTexture = Texture2D::create(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

	std::vector<int32_t> samplers;
	samplers.resize(utils::maxTextureSlots);
	int32_t i = 0;
	for (auto & sampler: samplers)
		sampler = i++;

	auto &shLib = Renderer::getShaderLibrary();
	shLib.addFromStandardPath("texture");
	data.shader = shLib.get("texture");
	data.shader->bind();
	data.shader->setIntArray("u_Textures", samplers.data(), utils::maxTextureSlots);

	// Set all texture slots to 0
	data.textureSlots[0] = data.whiteTexture;
}

void Renderer2D::shutdown() {
	OWL_PROFILE_FUNCTION()

}

void Renderer2D::beginScene(const CameraOrtho &camera) {
	OWL_PROFILE_FUNCTION()

	data.shader->bind();
	data.shader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
	startBatch();
}

void Renderer2D::beginScene(const CameraEditor &camera) {
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
		return;// Nothing to draw
	OWL_CORE_ASSERT(!data.quadVertexBuf.empty(), "Bad data size in flush")
	data.quadVertexBuffer->setData(data.quadVertexBuf.data(),
								   static_cast<uint32_t>(data.quadVertexBuf.size() * sizeof(utils::QuadVertex)));
	for (uint32_t i = 0; i < data.textureSlotIndex; i++)
		data.textureSlots[i]->bind(i);
	RenderCommand::drawIndexed(data.quadVertexArray, data.quadIndexCount);
	data.stats.drawCalls++;
}

void Renderer2D::startBatch() {
	data.quadIndexCount = 0;
	//data.quadVertexBufferPtr = data.quadVertexBufferBase;
	data.quadVertexBuf.clear();
	data.quadVertexBuf.reserve(utils::maxVertices);

	data.textureSlotIndex = 1;
}

void Renderer2D::nextBatch() {
	flush();
	startBatch();
}
void Renderer2D::drawQuad(const Quad2DDataT &quadData) {
	OWL_PROFILE_FUNCTION()
	if (data.quadIndexCount >= utils::maxIndices)
		nextBatch();
	float textureIndex = 0.0f;
	if (quadData.texture != nullptr) {
		for (uint32_t i = 1; i < data.textureSlotIndex; i++) {
			if (*data.textureSlots[i] == *quadData.texture) {
				textureIndex = static_cast<float>(i);
				break;
			}
		}
		if (textureIndex == 0.0f) {
			if (data.textureSlotIndex >= utils::maxTextureSlots)
				nextBatch();
			textureIndex = static_cast<float>(data.textureSlotIndex);
			data.textureSlots[data.textureSlotIndex] = std::static_pointer_cast<Texture2D>(quadData.texture);
			data.textureSlotIndex++;
		}
	}
	for (size_t i = 0; i < utils::quadVertexCount; i++) {
		data.quadVertexBuf.emplace_back(utils::QuadVertex{
				.position = quadData.transform * utils::quadVertexPosition[i],
				.color = quadData.color,
				.texCoord = utils::textureCoords[i],
				.texIndex = textureIndex,
				.tilingFactor = quadData.tilingFactor,
				.entityID = quadData.entityID
		});
	}
	data.quadIndexCount += 6;

	data.stats.quadCount++;
}

void Renderer2D::drawQuad(const Quad2DData &quadData) {
	OWL_PROFILE_FUNCTION()

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), quadData.position) *
						  glm::rotate(glm::mat4(1.0f), glm::radians(quadData.rotation), {0.0f, 0.0f, 1.0f}) *
						  glm::scale(glm::mat4(1.0f), {quadData.size.x, quadData.size.y, 1.0f});
	drawQuad({
			.transform = transform,
			.color = quadData.color,
			.texture = quadData.texture,
			.tilingFactor = quadData.tilingFactor,
			.entityID = quadData.entityID,
	});
}

void Renderer2D::drawSprite(const glm::mat4 &transform, scene::component::SpriteRenderer &src, int entityID) {
	drawQuad({.transform = transform,
			  .color = src.color,
			  .entityID = entityID});
}

void Renderer2D::resetStats() {
	data.stats.drawCalls = 0;
	data.stats.quadCount = 0;
}

Renderer2D::Statistics Renderer2D::getStats() {
	return data.stats;
}

}// namespace owl::renderer
