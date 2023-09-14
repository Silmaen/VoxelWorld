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

#include "DrawData.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "UniformBuffer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace owl::renderer {

namespace utils {

constexpr uint32_t maxQuads = 20000;
constexpr size_t quadVertexCount = 4;
constexpr uint32_t maxVertices = maxQuads * quadVertexCount;
constexpr uint32_t maxIndices = maxQuads * 6;
constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
constexpr glm::vec4 quadVertexPositions[] = {{-0.5f, -0.5f, 0.0f, 1.0f},
											 {0.5f, -0.5f, 0.0f, 1.0f},
											 {0.5f, 0.5f, 0.0f, 1.0f},
											 {-0.5f, 0.5f, 0.0f, 1.0f}};

static uint32_t maxTextureSlots = 0;

/**
 * @brief Structure holding quad vertex information.
 */
struct QuadVertex {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoord;
	float texIndex;
	float tilingFactor;
	int entityID;
};

/**
 * @brief Structure holding circle vertex information.
 */
struct CircleVertex {
	glm::vec3 worldPosition;
	glm::vec3 localPosition;
	glm::vec4 color;
	float thickness;
	float fade;
	int entityID;
};

/**
 * @brief Structure holding line vertex information.
 */
struct LineVertex {
	glm::vec3 position;
	glm::vec4 color;
	int entityID;
};

template<typename VertexType>
struct VertexData {
	uint32_t indexCount = 0;
	std::vector<VertexType> vertexBuf;
};

template<typename VertexType>
static void resetDrawData(VertexData<VertexType> &data_) {
	data_.indexCount = 0;
	data_.vertexBuf.clear();
	data_.vertexBuf.reserve(utils::maxVertices);
}

/**
 * @brief Structure holding static internal data
 */
struct internalData {
	/// Quad Data
	VertexData<QuadVertex> quad;
	shared<DrawData> drawQuad;
	/// Circle Data
	VertexData<CircleVertex> circle;
	shared<DrawData> drawCircle;
	/// Line Data
	VertexData<LineVertex> line;
	shared<DrawData> drawLine;
	float lineWidth = 2.0f;

	// Textures Data
	/// One white texture for coloring
	shared<Texture2D> whiteTexture;
	/// Array of textures
	std::vector<shared<Texture2D>> textureSlots;
	/// next texture index
	uint32_t textureSlotIndex = 1;// 0 = white texture

	/// Statistics
	Renderer2D::Statistics stats;

	/// Camera Data
	struct CameraData {
		/// Camera projection
		glm::mat4 viewProjection;
	};
	CameraData cameraBuffer{};
	shared<UniformBuffer> cameraUniformBuffer;
};

glm::mat4 toTransform(const PRS &transform) {
	return glm::translate(glm::mat4(1.0f), transform.position) *
		   glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation), {0.0f, 0.0f, 1.0f}) *
		   glm::scale(glm::mat4(1.0f), {transform.size.x, transform.size.y, 1.0f});
}

}// namespace utils

static utils::internalData data;

void Renderer2D::init() {
	OWL_PROFILE_FUNCTION()

	std::vector<uint32_t> quadIndices;
	{
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
	}
	// quads
	data.drawQuad = DrawData::create();
	data.drawQuad->init({
								{"a_Position", ShaderDataType::Float3},
								{"a_Color", ShaderDataType::Float4},
								{"a_TexCoord", ShaderDataType::Float2},
								{"a_TexIndex", ShaderDataType::Float},
								{"a_TilingFactor", ShaderDataType::Float},
								{"a_EntityID", ShaderDataType::Int},
						},
						quadIndices, "renderer2D_quad");
	// circles
	data.drawCircle = DrawData::create();
	data.drawCircle->init({
								  {"a_WorldPosition", ShaderDataType::Float3},
								  {"a_LocalPosition", ShaderDataType::Float3},
								  {"a_Color", ShaderDataType::Float4},
								  {"a_Thickness", ShaderDataType::Float},
								  {"a_Fade", ShaderDataType::Float},
								  {"a_EntityID", ShaderDataType::Int},
						  },
						  quadIndices, "renderer2D_circle");
	// Lines
	data.drawLine = DrawData::create();
	data.drawLine->init({
								{"a_Position", ShaderDataType::Float3},
								{"a_Color", ShaderDataType::Float4},
								{"a_EntityID", ShaderDataType::Int},
						},
						quadIndices, "renderer2D_line");

	data.whiteTexture = Texture2D::create(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));


	// Set all texture slots to 0
	utils::maxTextureSlots = RenderCommand::getMaxTextureSlots();
	data.textureSlots.resize(utils::maxTextureSlots);
	data.textureSlots[0] = data.whiteTexture;
	data.cameraUniformBuffer = UniformBuffer::create(sizeof(utils::internalData::CameraData), 0);
	data.cameraUniformBuffer->bind();
}

void Renderer2D::shutdown() {
	OWL_PROFILE_FUNCTION()
	// clearing the internal data
	data.cameraUniformBuffer.reset();
	data.whiteTexture.reset();
	for (auto &text: data.textureSlots) {
		if (text == nullptr) continue;
		text.reset();
	}
	data.drawQuad.reset();
	data.drawCircle.reset();
	data.drawLine.reset();
}

void Renderer2D::beginScene(const CameraOrtho &camera) {
	OWL_PROFILE_FUNCTION()

	data.cameraBuffer.viewProjection = camera.getViewProjectionMatrix();
	data.cameraUniformBuffer->setData(&data.cameraBuffer, sizeof(utils::internalData::CameraData));
	startBatch();
}

void Renderer2D::beginScene(const CameraEditor &camera) {
	OWL_PROFILE_FUNCTION()

	data.cameraBuffer.viewProjection = camera.getViewProjection();
	data.cameraUniformBuffer->setData(&data.cameraBuffer, sizeof(utils::internalData::CameraData));
	startBatch();
}

void Renderer2D::beginScene(const Camera &camera, const glm::mat4 &transform) {
	OWL_PROFILE_FUNCTION()

	data.cameraBuffer.viewProjection = camera.getProjection() * glm::inverse(transform);
	data.cameraUniformBuffer->setData(&data.cameraBuffer, sizeof(utils::internalData::CameraData));
	startBatch();
}

void Renderer2D::endScene() {
	OWL_PROFILE_FUNCTION()

	flush();
}

void Renderer2D::flush() {
	if (data.quad.indexCount > 0) {
		data.drawQuad->setVertexData(data.quad.vertexBuf.data(),
									 static_cast<uint32_t>(data.quad.vertexBuf.size() * sizeof(utils::QuadVertex)));
		// bind textures
		for (uint32_t i = 0; i < data.textureSlotIndex; i++)
			data.textureSlots[i]->bind(i);
		// draw call
		RenderCommand::drawData(data.drawQuad, data.quad.indexCount);
		data.stats.drawCalls++;
	}
	if (data.circle.indexCount > 0) {
		data.drawCircle->setVertexData(data.circle.vertexBuf.data(),
									   static_cast<uint32_t>(data.circle.vertexBuf.size() * sizeof(utils::CircleVertex)));
		// draw call
		RenderCommand::drawData(data.drawCircle, data.circle.indexCount);
		data.stats.drawCalls++;
	}
	if (data.line.indexCount > 0) {
		data.drawLine->setVertexData(data.line.vertexBuf.data(),
									 static_cast<uint32_t>(data.line.vertexBuf.size() * sizeof(utils::LineVertex)));
		// draw call
		RenderCommand::drawData(data.drawLine, data.line.indexCount);
		data.stats.drawCalls++;
	}
}

void Renderer2D::startBatch() {
	utils::resetDrawData(data.quad);
	utils::resetDrawData(data.circle);
	utils::resetDrawData(data.line);

	data.textureSlotIndex = 1;
}

void Renderer2D::nextBatch() {
	flush();
	startBatch();
}

float Renderer2D::getLineWidth() {
	return data.lineWidth;
}

void Renderer2D::setLineWidth(float width) {
	data.lineWidth = width;
}

void Renderer2D::drawLine(const LineData &lineData) {
	data.line.vertexBuf.emplace_back(utils::LineVertex{lineData.point1, lineData.color, lineData.entityID});
	data.line.vertexBuf.emplace_back(utils::LineVertex{lineData.point2, lineData.color, lineData.entityID});
	data.line.indexCount += 2;
	data.stats.drawCalls++;
}

void Renderer2D::drawRect(const RectData &lineData) {
	glm::mat4 trans = lineData.transform.transform;
	std::vector<glm::vec3> points;
	static const std::vector<std::pair<uint8_t, uint8_t>> idx = {{0, 1}, {1, 2}, {2, 3}, {3, 0}};
	for (const auto &vtx: utils::quadVertexPositions)
		points.emplace_back(trans * vtx);
	for (const auto &[p1, p2]: idx)
		drawLine({points[p1], points[p2], lineData.color, lineData.entityID});
}

void Renderer2D::drawPolyLine(const PolyLineData &lineData) {
	if (lineData.points.size() < 2) {
		OWL_CORE_WARN("Too few points in the multiline with ID {}", lineData.entityID)
		return;
	}
	glm::mat4 trans = lineData.transform.transform;
	std::vector<glm::vec3> points;
	std::vector<std::pair<uint32_t, uint32_t>> link;
	uint32_t i = 0;
	for (const auto &vtx: lineData.points) {
		points.emplace_back(trans * glm::vec4{vtx.x, vtx.y, vtx.z, 1.f});
		if (i < lineData.points.size() - 1)
			link.emplace_back(i, i + 1);
		++i;
	}
	if (lineData.closed)
		link.emplace_back(lineData.points.size() - 1, 0);
	for (const auto &[p1, p2]: link)
		drawLine({points[p1], points[p2], lineData.color, lineData.entityID});
}

void Renderer2D::drawCircle(const CircleData &circleData) {
	OWL_PROFILE_FUNCTION()

	// TODO: implement for circles
	// if (data.circleIndexCount >= utils::maxIndices)
	// 	nextBatch();

	for (const auto &vtx: utils::quadVertexPositions) {
		data.circle.vertexBuf.emplace_back(utils::CircleVertex{
				.worldPosition = circleData.transform.transform * vtx,
				.localPosition = vtx * 2.0f,
				.color = circleData.color,
				.thickness = circleData.thickness,
				.fade = circleData.fade,
				.entityID = circleData.entityID});
	}

	data.circle.indexCount += 6;

	data.stats.quadCount++;
}

void Renderer2D::drawQuad(const Quad2DData &quadData) {
	OWL_PROFILE_FUNCTION()
	if (data.quad.indexCount >= utils::maxIndices)
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
		data.quad.vertexBuf.emplace_back(utils::QuadVertex{
				.position = quadData.transform.transform * utils::quadVertexPositions[i],
				.color = quadData.color,
				.texCoord = utils::textureCoords[i],
				.texIndex = textureIndex,
				.tilingFactor = quadData.tilingFactor,
				.entityID = quadData.entityID});
	}
	data.quad.indexCount += 6;

	data.stats.quadCount++;
}

void Renderer2D::drawSprite(const glm::mat4 &transform, scene::component::SpriteRenderer &src, int entityID) {
	drawQuad({.transform = transform,
			  .color = src.color,
			  .texture = src.texture,
			  .tilingFactor = src.tilingFactor,
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
