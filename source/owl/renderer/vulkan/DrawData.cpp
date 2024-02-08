/**
 * @file DrawData.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "DrawData.h"

#include "internal/VulkanHandler.h"
#include "renderer/Renderer.h"

namespace owl::renderer::vulkan {

DrawData::~DrawData() = default;

void DrawData::init(const BufferLayout &layout, const std::string &renderer, std::vector<uint32_t> &indices, const std::string &shaderName) {
	setShader(shaderName, renderer);
	vertexBuffer = mk_shared<VertexBuffer>(layout.getStride() * indices.size());
	vertexBuffer->setLayout(layout);
	indexBuffer = mk_shared<IndexBuffer>(indices.data(), indices.size());

	auto &vkh = internal::VulkanHandler::get();
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages = shader->getStagesInfo();

	const auto bindingDescription = vertexBuffer->getBindingDescription();
	const auto attributeDescriptions = vertexBuffer->getAttributeDescriptions();
	const VkPipelineVertexInputStateCreateInfo vertexInputInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.vertexBindingDescriptionCount = 1,
			.pVertexBindingDescriptions = &bindingDescription,
			.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
			.pVertexAttributeDescriptions = attributeDescriptions.data(),
	};
	if (pipelineId >= 0)
		vkh.popPipeline(pipelineId);
	pipelineId = vkh.pushPipeline(shader->getName(), shaderStages, vertexInputInfo);
	for (const auto &stage: shaderStages) {
		vkDestroyShaderModule(vkh.getDevice(), stage.module, nullptr);
	}
	if (pipelineId < 0) {
		OWL_CORE_WARN("Vulkan shader: Failed to register pipeline {}.", shader->getName())
	}
}
void DrawData::setShader(const std::string &shaderName, const std::string &renderer) {
	auto &shLib = Renderer::getShaderLibrary();
	if (!shLib.exists(shaderName, renderer))
		shLib.addFromStandardPath(shaderName, renderer);
	shader = static_pointer_cast<Shader>(shLib.get(shaderName, renderer));
}

void DrawData::bind() const {
	if (pipelineId < 0)
		return;
	auto &vkh = internal::VulkanHandler::get();
	vkh.bindPipeline(pipelineId);
	vertexBuffer->bind();
	indexBuffer->bind();
}

void DrawData::unbind() const {
}

void DrawData::setVertexData(const void *data, uint32_t size) {
	if (pipelineId < 0)
		return;
	vertexBuffer->setData(data, size);
}

uint32_t DrawData::getIndexCount() const {
	if (pipelineId < 0)
		return 0;
	return indexBuffer->getCount();
}

}// namespace owl::renderer::vulkan
