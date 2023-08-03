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
#include "UniformBuffer.h"
#include "VertexArray.h"
#include <glm/gtc/matrix_transform.hpp>

namespace owl::renderer {

namespace utils {

constexpr uint32_t maxQuads = 20000;
constexpr size_t quadVertexCount = 4;
constexpr uint32_t maxVertices = maxQuads * quadVertexCount;
constexpr uint32_t maxIndices = maxQuads * 6;
constexpr int32_t maxTextureSlots = 32;// TODO: RenderCaps
constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
constexpr glm::vec4 quadVertexPositions[] = {{-0.5f, -0.5f, 0.0f, 1.0f},
											 {0.5f, -0.5f, 0.0f, 1.0f},
											 {0.5f, 0.5f, 0.0f, 1.0f},
											 {-0.5f, 0.5f, 0.0f, 1.0f}};

/**
 * @brief Structure holding quad vertex informations
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
 * @brief Structure holding circle vertex informations
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
 * @brief Structure holding line vertex informations
 */
struct LineVertex {
	glm::vec3 position;
	glm::vec4 color;
	int entityID;
};

/**
 * @brief Base structure for rendering an object type
 * @tparam VertexType Object's vertex data
 */
template<typename VertexType>
struct DrawData {
	shared<VertexArray> vertexArray;
	shared<VertexBuffer> vertexBuffer;
	shared<Shader> shader;
	uint32_t indexCount = 0;
	std::vector<VertexType> vertexBuf;
};


template<typename VertexType>
static void resetDrawData(DrawData<VertexType> &data_) {
	data_.indexCount = 0;
	data_.vertexBuf.clear();
	data_.vertexBuf.reserve(utils::maxVertices);
}

template<typename VertexType>
static void initDrawData(DrawData<VertexType> &data_, const BufferLayout &layout_, std::vector<uint32_t> &indices, const std::string &shaderName) {
	data_.vertexArray = VertexArray::create();
	data_.vertexBuffer = VertexBuffer::create(utils::maxVertices * sizeof(VertexType));
	data_.vertexBuffer->setLayout(layout_);
	data_.vertexArray->addVertexBuffer(data_.vertexBuffer);
	data_.vertexBuf.reserve(utils::maxVertices);
	data_.vertexArray->setIndexBuffer(IndexBuffer::create(indices.data(), utils::maxIndices));
	auto &shLib = Renderer::getShaderLibrary();
	shLib.addFromStandardPath(shaderName);
	data_.shader = shLib.get(shaderName);
}

/**
 * @brief Structure holding static internal data
 */
struct internalData {
	/// Quad Data
	DrawData<QuadVertex> quad;
	/// Circle Data
	DrawData<CircleVertex> circle;
	/// Line Data
	DrawData<LineVertex> line;
	float lineWidth = 2.0f;

	// Textures Data
	shared<Texture2D> whiteTexture;
	std::array<shared<Texture2D>, maxTextureSlots> textureSlots;
	uint32_t textureSlotIndex = 1;// 0 = white texture

	/// Statistics
	Renderer2D::Statistics stats;

	/// Camera Data
	struct CameraData {
		/// Camera projection
		glm::mat4 viewProjection;
	};
	CameraData cameraBuffer;
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
	utils::initDrawData(data.quad, {
										   {"a_Position", ShaderDataType::Float3},
										   {"a_Color", ShaderDataType::Float4},
										   {"a_TexCoord", ShaderDataType::Float2},
										   {"a_TexIndex", ShaderDataType::Float},
										   {"a_TilingFactor", ShaderDataType::Float},
										   {"a_EntityID", ShaderDataType::Int},
								   },
						quadIndices, "renderer2D_quad");
	data.quad.vertexArray->setIndexBuffer(IndexBuffer::create(quadIndices.data(), utils::maxIndices));
	// circles
	utils::initDrawData(data.circle, {
											 {"a_WorldPosition", ShaderDataType::Float3},
											 {"a_LocalPosition", ShaderDataType::Float3},
											 {"a_Color", ShaderDataType::Float4},
											 {"a_Thickness", ShaderDataType::Float},
											 {"a_Fade", ShaderDataType::Float},
											 {"a_EntityID", ShaderDataType::Int},
									 },
						quadIndices, "renderer2D_circle");
	// Lines
	utils::initDrawData(data.line, {
										   {"a_Position", ShaderDataType::Float3},
										   {"a_Color", ShaderDataType::Float4},
										   {"a_EntityID", ShaderDataType::Int},
								   },
						quadIndices, "renderer2D_line");

	data.whiteTexture = Texture2D::create(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

	std::vector<int32_t> samplers;
	samplers.resize(utils::maxTextureSlots);
	int32_t i = 0;
	for (auto &sampler: samplers)
		sampler = i++;

	// Set all texture slots to 0
	data.textureSlots[0] = data.whiteTexture;
	data.cameraUniformBuffer = UniformBuffer::create(sizeof(utils::internalData::CameraData), 0);
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
		data.quad.vertexBuffer->setData(data.quad.vertexBuf.data(),
										static_cast<uint32_t>(data.quad.vertexBuf.size() * sizeof(utils::QuadVertex)));
		// bind textures
		for (uint32_t i = 0; i < data.textureSlotIndex; i++)
			data.textureSlots[i]->bind(i);
		// bind shader
		data.quad.shader->bind();
		// draw call
		RenderCommand::drawIndexed(data.quad.vertexArray, data.quad.indexCount);
		data.stats.drawCalls++;
	}
	if (data.circle.indexCount > 0) {
		data.circle.vertexBuffer->setData(data.circle.vertexBuf.data(),
										  static_cast<uint32_t>(data.circle.vertexBuf.size() * sizeof(utils::CircleVertex)));
		// bind shader
		data.circle.shader->bind();
		// draw call
		RenderCommand::drawIndexed(data.circle.vertexArray, data.circle.indexCount);
		data.stats.drawCalls++;
	}
	if (data.line.indexCount > 0) {
		data.line.vertexBuffer->setData(data.line.vertexBuf.data(),
										static_cast<uint32_t>(data.line.vertexBuf.size() * sizeof(utils::CircleVertex)));
		// bind shader
		data.line.shader->bind();
		// draw call
		RenderCommand::drawIndexed(data.line.vertexArray, data.line.indexCount);
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
			link.emplace_back(std::pair<uint32_t, uint32_t>{i, i + 1});
		++i;
	}
	if (lineData.closed)
		link.emplace_back(std::pair<uint32_t, uint32_t>{lineData.points.size() - 1, 0});
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
