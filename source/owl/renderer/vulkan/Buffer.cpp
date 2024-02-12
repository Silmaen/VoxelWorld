/**
 * @file Buffer.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Buffer.h"

#include "internal/VulkanHandler.h"
#include "internal/utils.h"

namespace owl::renderer::vulkan {

namespace {

VkFormat shaderDataTypeToVulkanFormat(const ShaderDataType &type) {
	switch (type) {
		case ShaderDataType::None:
			return VK_FORMAT_UNDEFINED;
		case ShaderDataType::Float:
			return VK_FORMAT_R32_SFLOAT;
		case ShaderDataType::Float2:
			return VK_FORMAT_R32G32_SFLOAT;
		case ShaderDataType::Float3:
			return VK_FORMAT_R32G32B32_SFLOAT;
		case ShaderDataType::Float4:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
			return VK_FORMAT_R32_SFLOAT;
		case ShaderDataType::Int:
			return VK_FORMAT_R32_SINT;
		case ShaderDataType::Int2:
			return VK_FORMAT_R32G32_SINT;
		case ShaderDataType::Int3:
			return VK_FORMAT_R32G32B32_SINT;
		case ShaderDataType::Int4:
			return VK_FORMAT_R32G32B32A32_SINT;
		case ShaderDataType::Bool:
			return VK_FORMAT_R8_UINT;
	}
	return VK_FORMAT_UNDEFINED;
}

}// namespace

VertexBuffer::VertexBuffer(const uint32_t size) {
	createBuffer(nullptr, size);
}

VertexBuffer::VertexBuffer(const float *vertices, const uint32_t size) {
	createBuffer(vertices, size);
}

VertexBuffer::~VertexBuffer() {
	release();
}

void VertexBuffer::release() {
	const auto &vkh = internal::VulkanHandler::get();
	if (vkh.getState() != internal::VulkanHandler::State::Running) {
		OWL_CORE_WARN("Vulkan vertex buffer: Trying to delete vertex buffer after VulkanHander release...")
		return;
	}
	const auto &vkc = internal::VulkanCore::get();
	if (vertexBuffer != nullptr)
		vkDestroyBuffer(vkc.getLogicalDevice(), vertexBuffer, nullptr);
	vertexBuffer = nullptr;
	if (vertexBufferMemory != nullptr)
		vkFreeMemory(vkc.getLogicalDevice(), vertexBufferMemory, nullptr);
	vertexBufferMemory = nullptr;
}

void VertexBuffer::bind() const {
	const auto &vkh = internal::VulkanHandler::get();
	if (vkh.getState() != internal::VulkanHandler::State::Running) {
		OWL_CORE_WARN("Vulkan vertex buffer: Trying to bind vertex buffer after VulkanHander release...")
		return;
	}
	const VkBuffer vertexBuffers[] = {vertexBuffer};
	constexpr VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(vkh.getCurrentCommandBuffer(), 0, 1, vertexBuffers, offsets);
}

void VertexBuffer::unbind() const {}

void VertexBuffer::setData(const void *data, const uint32_t size) {
	const auto &vkh = internal::VulkanHandler::get();
	if (vkh.getState() != internal::VulkanHandler::State::Running) {
		OWL_CORE_WARN("Vulkan vertex buffer: Trying to set vertex buffer data after VulkanHander release...")
		return;
	}

	if (data != nullptr) {
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		vkh.createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
		const auto &vkc = internal::VulkanCore::get();

		void *data_internal;
		vkMapMemory(vkc.getLogicalDevice(), stagingBufferMemory, 0, size, 0, &data_internal);
		memcpy(data_internal, data, size);
		vkUnmapMemory(vkc.getLogicalDevice(), stagingBufferMemory);

		vkh.copyBuffer(stagingBuffer, vertexBuffer, size);

		vkDestroyBuffer(vkc.getLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(vkc.getLogicalDevice(), stagingBufferMemory, nullptr);
	}
}

VkVertexInputBindingDescription VertexBuffer::getBindingDescription() const {
	return {
			.binding = 0,
			.stride = getLayout().getStride(),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX};
}

std::vector<VkVertexInputAttributeDescription> VertexBuffer::getAttributeDescriptions() const {
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

	auto layout = getLayout();
	uint32_t id = 0;
	for (const auto &element: layout) {
		attributeDescriptions.emplace_back(VkVertexInputAttributeDescription{
				.location = id,
				.binding = 0,
				.format = shaderDataTypeToVulkanFormat(element.type),
				.offset = element.offset,
		});
		++id;
	}
	return attributeDescriptions;
}

void VertexBuffer::createBuffer(const float *data, const uint32_t size) {
	const auto &vkh = internal::VulkanHandler::get();
	if (vkh.getState() != internal::VulkanHandler::State::Running) {
		OWL_CORE_WARN("Vulkan vertex buffer: Trying to set vertex buffer data after VulkanHander release...")
		return;
	}
	release();
	vkh.createBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
	setData(data, size);
}


IndexBuffer::IndexBuffer(uint32_t *data, uint32_t size) : count(size) {
	const auto &vkh = internal::VulkanHandler::get();
	if (vkh.getState() != internal::VulkanHandler::State::Running) {
		OWL_CORE_WARN("Vulkan index buffer: Trying to create index buffer data after VulkanHander release...")
		return;
	}
	const VkDeviceSize bufferSize = sizeof(uint32_t) * size;
	vkh.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	if (data != nullptr) {
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		vkh.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
		void *data_internal;
		const auto &vkc = internal::VulkanCore::get();

		vkMapMemory(vkc.getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data_internal);
		memcpy(data_internal, data, bufferSize);
		vkUnmapMemory(vkc.getLogicalDevice(), stagingBufferMemory);
		vkh.copyBuffer(stagingBuffer, indexBuffer, bufferSize);
		vkDestroyBuffer(vkc.getLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(vkc.getLogicalDevice(), stagingBufferMemory, nullptr);
	}
}

IndexBuffer::~IndexBuffer() {
	release();
}

void IndexBuffer::release() {
	const auto &vkh = internal::VulkanHandler::get();
	if (vkh.getState() != internal::VulkanHandler::State::Running) {
		OWL_CORE_WARN("Vulkan vertex buffer: Trying to delete vertex buffer after VulkanHander release...")
		return;
	}
	const auto &vkc = internal::VulkanCore::get();

	if (indexBuffer != nullptr)
		vkDestroyBuffer(vkc.getLogicalDevice(), indexBuffer, nullptr);
	indexBuffer = nullptr;
	if (indexBufferMemory != nullptr)
		vkFreeMemory(vkc.getLogicalDevice(), indexBufferMemory, nullptr);
	indexBufferMemory = nullptr;
	count = 0;
}

void IndexBuffer::bind() const {
	const auto &vkh = internal::VulkanHandler::get();
	if (vkh.getState() != internal::VulkanHandler::State::Running) {
		OWL_CORE_WARN("Vulkan vertex buffer: Trying to bind vertex buffer after VulkanHander release...")
		return;
	}
	vkCmdBindIndexBuffer(vkh.getCurrentCommandBuffer(), indexBuffer, 0, VK_INDEX_TYPE_UINT16);
}

void IndexBuffer::unbind() const {}

}// namespace owl::renderer::vulkan
