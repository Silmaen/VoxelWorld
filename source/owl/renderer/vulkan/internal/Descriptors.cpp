/**
 * @file Descriptors.cpp
 * @author Silmaen
 * @date 13/03/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Descriptors.h"

#include "VulkanCore.h"
#include "VulkanHandler.h"
#include "utils.h"

namespace owl::renderer::vulkan::internal {

void TextureData::freeTrexture() {
	const auto &core = internal::VulkanCore::get();
	if (m_textureSampler) {
		vkDestroySampler(core.getLogicalDevice(), m_textureSampler, nullptr);
		m_textureSampler = nullptr;
	}
	if (m_textureImageView) {
		vkDestroyImageView(core.getLogicalDevice(), m_textureImageView, nullptr);
		m_textureImageView = nullptr;
	}
	if (m_textureImage) {
		vkDestroyImage(core.getLogicalDevice(), m_textureImage, nullptr);
		m_textureImage = nullptr;
	}
	if (m_textureImageMemory) {
		vkFreeMemory(core.getLogicalDevice(), m_textureImageMemory, nullptr);
		m_textureImageMemory = nullptr;
	}
}

Descriptors::Descriptors() = default;

Descriptors::~Descriptors() { release(); }

void Descriptors::release() {
	const auto &core = VulkanCore::get();
	if (!m_uniformBuffers.empty()) {
		for (size_t i = 0; i < g_maxFrameInFlight; ++i) {
			vkDestroyBuffer(core.getLogicalDevice(), m_uniformBuffers[i], nullptr);
			vkFreeMemory(core.getLogicalDevice(), m_uniformBuffersMemory[i], nullptr);
		}
		m_uniformBuffers.clear();
		m_uniformBuffersMemory.clear();
	}
	if (m_imguiDescriptorPool != nullptr) {
		vkDestroyDescriptorPool(core.getLogicalDevice(), m_imguiDescriptorPool, nullptr);
		m_imguiDescriptorPool = nullptr;
		OWL_CORE_TRACE("Vulkan: descriptorPool destroyed.")
	}
	if (m_descriptorSetLayout != nullptr) {
		vkDestroyDescriptorSetLayout(core.getLogicalDevice(), m_descriptorSetLayout, nullptr);
		m_descriptorSetLayout = nullptr;
		OWL_CORE_TRACE("Vulkan: descriptorSetLayout destroyed.")
	}
	if (m_descriptorPool != nullptr) {
		vkDestroyDescriptorPool(core.getLogicalDevice(), m_descriptorPool, nullptr);
		m_descriptorPool = nullptr;
		OWL_CORE_TRACE("Vulkan: descriptorPool destroyed.")
	}
}

void Descriptors::createDescriptors() {
	const auto &core = VulkanCore::get();

	// Descriptor pools.
	std::vector<VkDescriptorPoolSize> poolSizes{{
														.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
														.descriptorCount = static_cast<uint32_t>(g_maxFrameInFlight)},
												{
														.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
														.descriptorCount = static_cast<uint32_t>(g_maxFrameInFlight)},
	};
	const VkDescriptorPoolCreateInfo poolInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.maxSets = static_cast<uint32_t>(g_maxFrameInFlight),
			.poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
			.pPoolSizes = poolSizes.data()};
	if (const VkResult result = vkCreateDescriptorPool(core.getLogicalDevice(), &poolInfo, nullptr, &m_descriptorPool);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Descriptors: failed to create descriptor pool ({})", resultString(result))
	}

	// Descriptor sets layout
	constexpr VkDescriptorSetLayoutBinding uboLayoutBinding{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
			.pImmutableSamplers = nullptr};
	constexpr VkDescriptorSetLayoutBinding samplerLayoutBinding{
			.binding = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = 32,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
			.pImmutableSamplers = nullptr
	};
	std::vector bindings = {uboLayoutBinding, samplerLayoutBinding};
	const VkDescriptorSetLayoutCreateInfo layoutInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.bindingCount = static_cast<uint32_t>(bindings.size()),
			.pBindings = bindings.data()};
	if (const auto result =
				vkCreateDescriptorSetLayout(core.getLogicalDevice(), &layoutInfo, nullptr, &m_descriptorSetLayout);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Descriptor: failed to create descriptor set layout ({}).", resultString(result))
	}

	// Descriptor sets
	std::vector<VkDescriptorSetLayout> layouts(g_maxFrameInFlight, m_descriptorSetLayout);
	const VkDescriptorSetAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext = nullptr,
			.descriptorPool = m_descriptorPool,
			.descriptorSetCount = static_cast<uint32_t>(g_maxFrameInFlight),
			.pSetLayouts = layouts.data()};
	m_descriptorSets.resize(g_maxFrameInFlight);
	if (const auto result = vkAllocateDescriptorSets(core.getLogicalDevice(), &allocInfo, m_descriptorSets.data());
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Descriptor: failed to allocate descriptor sets ({})", resultString(result))
	}
}

void Descriptors::updateDescriptors() { for (size_t i = 0; i < g_maxFrameInFlight; i++) { updateDescriptor(i); } }

void Descriptors::updateDescriptor(const size_t iFrame) {
	const auto &core = VulkanCore::get();
	std::vector<VkDescriptorBufferInfo> bufferInfos;
	if (!m_uniformBuffers.empty()) {
		bufferInfos.push_back({
				.buffer = m_uniformBuffers[iFrame],
				.offset = 0,
				.range = m_uniformSize});
	}
	std::vector<VkDescriptorImageInfo> imageInfos;
	imageInfos.reserve(m_textureBind.size());
	for (const auto &id: m_textureBind) {
		imageInfos.push_back({
				.sampler = m_textures[id].m_textureSampler,
				.imageView = m_textures[id].m_textureImageView,
				.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		});
	}
	std::vector<VkWriteDescriptorSet> descriptorWrites;
	if (!bufferInfos.empty()) {
		descriptorWrites.push_back({
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext = nullptr,
				.dstSet = m_descriptorSets[iFrame],
				.dstBinding = 0,
				.dstArrayElement = 0,
				.descriptorCount = static_cast<uint32_t>(bufferInfos.size()),
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.pImageInfo = nullptr,
				.pBufferInfo = bufferInfos.data(),
				.pTexelBufferView = nullptr
		});
	}
	if (!imageInfos.empty()) {
		descriptorWrites.push_back({
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext = nullptr,
				.dstSet = m_descriptorSets[iFrame],
				.dstBinding = 1,
				.dstArrayElement = 0,
				.descriptorCount = static_cast<uint32_t>(imageInfos.size()),
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.pImageInfo = imageInfos.data(),
				.pBufferInfo = nullptr,
				.pTexelBufferView = nullptr
		});
	}
	if (!descriptorWrites.empty()) {
		vkUpdateDescriptorSets(core.getLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()),
							   descriptorWrites.data(), 0, nullptr);
	}
}

void Descriptors::registerUniform(const uint32_t iSize) {
	const auto &vkh = VulkanHandler::get();
	m_uniformBuffers.resize(g_maxFrameInFlight);
	m_uniformBuffersMemory.resize(g_maxFrameInFlight);
	m_uniformBuffersMapped.resize(g_maxFrameInFlight);
	m_uniformSize = iSize;
	const auto &core = VulkanCore::get();
	for (size_t i = 0; i < g_maxFrameInFlight; i++) {
		vkh.createBuffer(iSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
						 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
						 m_uniformBuffers[i],
						 m_uniformBuffersMemory[i]);
		if (const auto result = vkMapMemory(core.getLogicalDevice(), m_uniformBuffersMemory[i], 0, iSize, 0,
											&m_uniformBuffersMapped[i]); result != VK_SUCCESS)
			OWL_CORE_ERROR("Vulkan: Failed to create uniform buffer {}.", i)
	}
}

void Descriptors::setUniformData(const void *iData, const size_t iSize) const {
	const auto &vkh = VulkanHandler::get();
	memcpy(m_uniformBuffersMapped[vkh.getCurrentFrame()], iData, iSize);
}

uint32_t Descriptors::registerNewTexture() {
	++nextId;
	m_textures[nextId] = TextureData{};
	return nextId;
}

TextureData &Descriptors::getTextureData(const uint32_t iIndex) { return m_textures.at(iIndex); }

void Descriptors::unregisterTexture(const uint32_t iIndex) {
	m_textures[iIndex].freeTrexture();
	m_textures.erase(iIndex);
}

void Descriptors::bindTextureImage(const uint32_t iIndex) {
	if (iIndex == m_bindedTexture)
		return;
	vkBindImageMemory(VulkanCore::get().getLogicalDevice(), m_textures[iIndex].m_textureImage,
					  m_textures[iIndex].m_textureImageMemory, 0);
	m_bindedTexture = iIndex;
}

void Descriptors::resetTextureBind() { m_textureBind.clear(); }

void Descriptors::commitTextureBind(const size_t iCurrentFrame) { updateDescriptor(iCurrentFrame); }

void Descriptors::textureBind(const uint32_t iIndex) { m_textureBind.push_back(iIndex); }

void Descriptors::createImguiDescriptorPool() {
	if (m_imguiDescriptorPool)
		return;
	const auto &core = VulkanCore::get();
	// Descriptor pools.
	std::vector<VkDescriptorPoolSize> poolSizes{{
														.type = VK_DESCRIPTOR_TYPE_SAMPLER,
														.descriptorCount = 1000},
												{
														.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
														.descriptorCount = 1000},
												{
														.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
														.descriptorCount = 1000},
												{
														.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
														.descriptorCount = 1000},
												{
														.type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
														.descriptorCount = 1000},
												{
														.type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
														.descriptorCount = 1000},
												{
														.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
														.descriptorCount = 1000},
												{
														.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
														.descriptorCount = 1000},
												{
														.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
														.descriptorCount = 1000},
												{
														.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
														.descriptorCount = 1000},
												{
														.type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
														.descriptorCount = 1000},
	};
	const VkDescriptorPoolCreateInfo poolInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
			.maxSets = static_cast<uint32_t>(g_maxFrameInFlight),
			.poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
			.pPoolSizes = poolSizes.data()};
	if (const VkResult result = vkCreateDescriptorPool(core.getLogicalDevice(), &poolInfo, nullptr,
													   &m_imguiDescriptorPool);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Descriptors: failed to create descriptor pool ({})", resultString(result))
	}
}


}// internal
