/**
 * @file Descriptors.cpp
 * @author Silmaen
 * @date 13/03/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Descriptors.h"

#include "VulkanCore.h"
#include "VulkanHandler.h"
#include "utils.h"

namespace owl::renderer::vulkan::internal {


void TextureData::freeTrexture() {
	const auto &core = internal::VulkanCore::get();
	const auto &pool = Descriptors::get().getSingleImageDescriptorPool();
	vkDeviceWaitIdle(core.getLogicalDevice());
	if (textureDescriptorSet != nullptr) {
		vkFreeDescriptorSets(core.getLogicalDevice(), pool, 1, &textureDescriptorSet);
		textureDescriptorSet = nullptr;
	}
	if (textureDescriptorSetLayout != nullptr) {
		vkDestroyDescriptorSetLayout(core.getLogicalDevice(), textureDescriptorSetLayout, nullptr);
		textureDescriptorSetLayout = nullptr;
	}
	if (textureSampler) {
		vkDestroySampler(core.getLogicalDevice(), textureSampler, nullptr);
		textureSampler = nullptr;
	}
	if (textureImageView) {
		vkDestroyImageView(core.getLogicalDevice(), textureImageView, nullptr);
		textureImageView = nullptr;
	}
	if (textureImage) {
		vkDestroyImage(core.getLogicalDevice(), textureImage, nullptr);
		textureImage = nullptr;
	}
	if (textureImageMemory) {
		vkFreeMemory(core.getLogicalDevice(), textureImageMemory, nullptr);
		textureImageMemory = nullptr;
	}
}

void TextureData::createDescriptorSet() {
	const auto &pool = Descriptors::get().getSingleImageDescriptorPool();
	const auto &core = VulkanCore::get();
	static constexpr VkDescriptorSetLayoutBinding samplerLayoutBinding{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
			.pImmutableSamplers = nullptr};
	constexpr VkDescriptorSetLayoutCreateInfo layoutCi{.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
													   .pNext = nullptr,
													   .flags = {},
													   .bindingCount = 1,
													   .pBindings = &samplerLayoutBinding};
	if (const auto result =
				vkCreateDescriptorSetLayout(core.getLogicalDevice(), &layoutCi, nullptr, &textureDescriptorSetLayout);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Texture Descriptor: failed to create descriptor set layout ({}).", resultString(result))
	}
	const VkDescriptorSetAllocateInfo allocInfo{.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
												.pNext = nullptr,
												.descriptorPool = pool,
												.descriptorSetCount = 1,
												.pSetLayouts = &textureDescriptorSetLayout};
	if (const auto result = vkAllocateDescriptorSets(core.getLogicalDevice(), &allocInfo, &textureDescriptorSet);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Texture Descriptor: failed to allocate descriptor sets ({})", resultString(result))
	}
	const VkDescriptorImageInfo info{.sampler = textureSampler,
									 .imageView = textureImageView,
									 .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	const VkWriteDescriptorSet wrt{.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
								   .pNext = nullptr,
								   .dstSet = textureDescriptorSet,
								   .dstBinding = 0,
								   .dstArrayElement = 0,
								   .descriptorCount = 1,
								   .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
								   .pImageInfo = &info,
								   .pBufferInfo = nullptr,
								   .pTexelBufferView = nullptr};
	vkUpdateDescriptorSets(core.getLogicalDevice(), 1, &wrt, 0, nullptr);
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
	if (m_singleImageDescriptorPool != nullptr) {
		vkDestroyDescriptorPool(core.getLogicalDevice(), m_singleImageDescriptorPool, nullptr);
		m_singleImageDescriptorPool = nullptr;
	}
	if (m_imguiDescriptorPool != nullptr) {
		vkDestroyDescriptorPool(core.getLogicalDevice(), m_imguiDescriptorPool, nullptr);
		m_imguiDescriptorPool = nullptr;
	}
	if (m_descriptorSetLayout != nullptr) {
		vkDestroyDescriptorSetLayout(core.getLogicalDevice(), m_descriptorSetLayout, nullptr);
		m_descriptorSetLayout = nullptr;
	}
	if (m_descriptorPool != nullptr) {
		vkDestroyDescriptorPool(core.getLogicalDevice(), m_descriptorPool, nullptr);
		m_descriptorPool = nullptr;
	}
}

void Descriptors::createDescriptors() {
	const auto &core = VulkanCore::get();

	// Descriptor pools.
	std::vector<VkDescriptorPoolSize> poolSizes{
			{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = static_cast<uint32_t>(g_maxFrameInFlight)},
			{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			 .descriptorCount = static_cast<uint32_t>(g_maxFrameInFlight)},
	};
	const VkDescriptorPoolCreateInfo poolInfo{.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
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
	constexpr VkDescriptorSetLayoutBinding uboLayoutBinding{.binding = 0,
															.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
															.descriptorCount = 1,
															.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
															.pImmutableSamplers = nullptr};
	constexpr VkDescriptorSetLayoutBinding samplerLayoutBinding{.binding = 1,
																.descriptorType =
																		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
																.descriptorCount = 32,
																.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
																.pImmutableSamplers = nullptr};
	std::vector bindings = {uboLayoutBinding, samplerLayoutBinding};
	const VkDescriptorSetLayoutCreateInfo layoutInfo{.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
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
	const VkDescriptorSetAllocateInfo allocInfo{.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
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

void Descriptors::updateDescriptors() {
	for (size_t i = 0; i < g_maxFrameInFlight; i++) { updateDescriptor(i); }
}

void Descriptors::updateDescriptor(const size_t iFrame) {
	const auto &core = VulkanCore::get();
	std::vector<VkDescriptorBufferInfo> bufferInfos;
	if (!m_uniformBuffers.empty()) {
		bufferInfos.push_back({.buffer = m_uniformBuffers[iFrame], .offset = 0, .range = m_uniformSize});
	}
	std::vector<VkDescriptorImageInfo> imageInfos;
	imageInfos.reserve(m_textureBind.size());
	for (const auto &id: m_textureBind) {
		imageInfos.push_back({.sampler = m_textures[id].textureSampler,
							  .imageView = m_textures[id].textureImageView,
							  .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL});
	}
	std::vector<VkWriteDescriptorSet> descriptorWrites;
	if (!bufferInfos.empty()) {
		descriptorWrites.push_back({.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
									.pNext = nullptr,
									.dstSet = m_descriptorSets[iFrame],
									.dstBinding = 0,
									.dstArrayElement = 0,
									.descriptorCount = static_cast<uint32_t>(bufferInfos.size()),
									.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
									.pImageInfo = nullptr,
									.pBufferInfo = bufferInfos.data(),
									.pTexelBufferView = nullptr});
	}
	if (!imageInfos.empty()) {
		descriptorWrites.push_back({.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
									.pNext = nullptr,
									.dstSet = m_descriptorSets[iFrame],
									.dstBinding = 1,
									.dstArrayElement = 0,
									.descriptorCount = static_cast<uint32_t>(imageInfos.size()),
									.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
									.pImageInfo = imageInfos.data(),
									.pBufferInfo = nullptr,
									.pTexelBufferView = nullptr});
	}
	if (!descriptorWrites.empty()) {
		vkUpdateDescriptorSets(core.getLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()),
							   descriptorWrites.data(), 0, nullptr);
	}
}

void Descriptors::registerUniform(const uint32_t iSize) {
	m_uniformBuffers.resize(g_maxFrameInFlight);
	m_uniformBuffersMemory.resize(g_maxFrameInFlight);
	m_uniformBuffersMapped.resize(g_maxFrameInFlight);
	m_uniformSize = iSize;
	const auto &core = VulkanCore::get();
	for (size_t i = 0; i < g_maxFrameInFlight; i++) {
		createBuffer(iSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
					 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i],
					 m_uniformBuffersMemory[i]);
		if (const auto result = vkMapMemory(core.getLogicalDevice(), m_uniformBuffersMemory[i], 0, iSize, 0,
											&m_uniformBuffersMapped[i]);
			result != VK_SUCCESS)
			OWL_CORE_ERROR("Vulkan: Failed to create uniform buffer {}.", i)
	}
}

void Descriptors::setUniformData(const void *iData, const size_t iSize) const {
	const auto &vkh = VulkanHandler::get();
	memcpy(m_uniformBuffersMapped[vkh.getCurrentFrameIndex()], iData, iSize);
}

uint32_t Descriptors::registerNewTexture() {
	++m_nextId;
	m_textures[m_nextId] = TextureData{};
	return m_nextId;
}

TextureData &Descriptors::getTextureData(const uint32_t iIndex) { return m_textures.at(iIndex); }

void Descriptors::unregisterTexture(const uint32_t iIndex) {
	m_textures[iIndex].freeTrexture();
	m_textures.erase(iIndex);
}

void Descriptors::bindTextureImage(const uint32_t iIndex) {
	if (iIndex == m_bindedTexture)
		return;
	vkBindImageMemory(VulkanCore::get().getLogicalDevice(), m_textures[iIndex].textureImage,
					  m_textures[iIndex].textureImageMemory, 0);
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
	std::vector<VkDescriptorPoolSize> poolSizes{
			{.type = VK_DESCRIPTOR_TYPE_SAMPLER, .descriptorCount = 1000},
			{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 1000},
			{.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, .descriptorCount = 1000},
			{.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount = 1000},
			{.type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, .descriptorCount = 1000},
			{.type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, .descriptorCount = 1000},
			{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1000},
			{.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, .descriptorCount = 1000},
			{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, .descriptorCount = 1000},
			{.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, .descriptorCount = 1000},
			{.type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, .descriptorCount = 1000},
	};
	const VkDescriptorPoolCreateInfo poolInfo{.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
											  .pNext = nullptr,
											  .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
											  .maxSets = static_cast<uint32_t>(g_maxFrameInFlight),
											  .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
											  .pPoolSizes = poolSizes.data()};
	if (const VkResult result =
				vkCreateDescriptorPool(core.getLogicalDevice(), &poolInfo, nullptr, &m_imguiDescriptorPool);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Descriptors: failed to create descriptor pool ({})", resultString(result))
	}
}

void Descriptors::createSingleImageDescriptorPool() {
	if (m_singleImageDescriptorPool)
		return;
	const auto &core = VulkanCore::get();
	// Descriptor pools.
	std::vector<VkDescriptorPoolSize> poolSizes{
			{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 1000},
	};
	const VkDescriptorPoolCreateInfo poolInfo{.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
											  .pNext = nullptr,
											  .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
											  .maxSets = 1024,
											  .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
											  .pPoolSizes = poolSizes.data()};
	if (const VkResult result =
				vkCreateDescriptorPool(core.getLogicalDevice(), &poolInfo, nullptr, &m_singleImageDescriptorPool);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Descriptors: failed to create descriptor pool ({})", resultString(result))
	}
}


}// namespace owl::renderer::vulkan::internal
