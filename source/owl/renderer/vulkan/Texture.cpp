/**
 * @file Texture.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Texture.h"

#include "internal/Descriptors.h"
#include "internal/VulkanHandler.h"
#include "internal/utils.h"

#include <stb_image.h>

namespace owl::renderer::vulkan {

namespace {

void createImage(const uint32_t iIndex, const math::FrameSize &iDimensions) {
	const auto &vkc = internal::VulkanCore::get();
	const VkImageCreateInfo imageInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_R8G8B8A8_UNORM,
			.extent = {.width = iDimensions.getWidth(), .height = iDimensions.getHeight(), .depth = 1},
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
			         VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = nullptr,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED};

	auto &data = internal::Descriptors::get().getTextureData(iIndex);
	data.freeTrexture();
	if (const VkResult result = vkCreateImage(vkc.getLogicalDevice(), &imageInfo, nullptr, &data.textureImage);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Texture: failed to create image ({}).", internal::resultString(result))
		return;
	}
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(vkc.getLogicalDevice(), data.textureImage, &memRequirements);
	const VkMemoryAllocateInfo allocInfo{.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
	                                     .pNext = nullptr,
	                                     .allocationSize = memRequirements.size,
	                                     .memoryTypeIndex = vkc.findMemoryTypeIndex(
			                                     memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)};
	if (const VkResult result =
				vkAllocateMemory(vkc.getLogicalDevice(), &allocInfo, nullptr, &data.textureImageMemory);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Texture: failed to allocate image memory ({}).", internal::resultString(result))
		return;
	}
	internal::Descriptors::get().bindTextureImage(iIndex);
}

}// namespace

Texture2D::Texture2D(const math::FrameSize &iSize, const bool iWithAlpha) : m_size{iSize}, m_hasAlpha{iWithAlpha} {}

Texture2D::Texture2D(const uint32_t iWidth, const uint32_t iHeight, const bool iWithAlpha)
	: m_size{iWidth, iHeight}, m_hasAlpha{iWithAlpha} {}

Texture2D::Texture2D(std::filesystem::path iPath) : m_path{std::move(iPath)} {
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc *data;
	{
		OWL_PROFILE_SCOPE("stbi_load - vulkan::Texture2D::Texture2D(const std::filesystem::path &)")
		data = stbi_load(m_path.string().c_str(), &width, &height, &channels, 0);
	}
	if (!data) {
		OWL_CORE_WARN("Vulkan Texture: Failed to load image {}", m_path.string())
		return;
	}

	m_hasAlpha = true;
	if (channels == 3)
		m_hasAlpha = false;
	if ((channels != 4) && (channels != 3)) {
		OWL_CORE_ERROR("Vulkan Texture: Impossible to load {}, invalid number of channels {}: must be 3 or 4.")
		return;
	}
	m_size = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
	setData(data, m_size.surface() * static_cast<uint32_t>(channels));

	stbi_image_free(data);
}

Texture2D::~Texture2D() {
	if (m_textureId)
		internal::Descriptors::get().unregisterTexture(m_textureId);
}

bool Texture2D::operator==(const Texture &iOther) const {
	const auto &bob = dynamic_cast<const Texture2D &>(iOther);
	return bob.m_textureId == m_textureId;
}

void Texture2D::bind(uint32_t) const { internal::Descriptors::get().textureBind(m_textureId); }

void Texture2D::setData(void *iData, const uint32_t iSize) {
	const auto &vkc = internal::VulkanCore::get();
	if (const uint32_t expected = m_hasAlpha ? m_size.surface() * 4 : m_size.surface() * 3; iSize != expected) {
		OWL_CORE_ERROR("Vulkan Texture {}: Image size missmatch: expect {}, got {}", m_path.string(), expected, iSize)
		return;
	}
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	const VkDeviceSize imageSize = m_size.surface() * 4;
	internal::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
	                       stagingBufferMemory);
	void *dataPixel;
	vkMapMemory(vkc.getLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &dataPixel);
	if (m_hasAlpha) {
		// input data already in the right format, just copy
		memcpy(dataPixel, iData, imageSize);
	} else {
		// need to insert alpha chanel.
		const auto *dataChar = static_cast<uint8_t *>(iData);
		auto *dataPixelChar = static_cast<uint8_t *>(dataPixel);
		for (uint32_t i = 0, j = 0; j < iSize; i += 4, j += 3) {
			memcpy(dataPixelChar + i, dataChar + j, 3);
			*(dataPixelChar + i + 3) = 0xFFu;
		}
	}
	vkUnmapMemory(vkc.getLogicalDevice(), stagingBufferMemory);
	auto &vkd = internal::Descriptors::get();
	m_textureId = vkd.registerNewTexture();
	auto &data = vkd.getTextureData(m_textureId);
	createImage(m_textureId, m_size);
	internal::transitionImageLayout(data.textureImage, VK_IMAGE_LAYOUT_UNDEFINED,
	                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	internal::copyBufferToImage(stagingBuffer, data.textureImage, m_size);
	internal::transitionImageLayout(data.textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	                                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(vkc.getLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(vkc.getLogicalDevice(), stagingBufferMemory, nullptr);

	const VkImageViewCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.image = data.textureImage,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = VK_FORMAT_R8G8B8A8_UNORM,
			.components = {VK_COMPONENT_SWIZZLE_IDENTITY,
			               VK_COMPONENT_SWIZZLE_IDENTITY,
			               VK_COMPONENT_SWIZZLE_IDENTITY,
			               VK_COMPONENT_SWIZZLE_IDENTITY},
			.subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			                     .baseMipLevel = 0,
			                     .levelCount = 1,
			                     .baseArrayLayer = 0,
			                     .layerCount = 1}};
	if (const VkResult result =
				vkCreateImageView(vkc.getLogicalDevice(), &createInfo, nullptr, &data.textureImageView);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Texture: Error creating image views ({}).", internal::resultString(result))
		return;
	}

	const VkSamplerCreateInfo samplerInfo{
			.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.magFilter = VK_FILTER_NEAREST,
			.minFilter = VK_FILTER_NEAREST,
			.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
			.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.mipLodBias = {},
			.anisotropyEnable = VK_TRUE,
			.maxAnisotropy = vkc.getMaxSamplerAnisotropy(),
			.compareEnable = VK_FALSE,
			.compareOp = VK_COMPARE_OP_ALWAYS,
			.minLod = {},
			.maxLod = {},
			.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
			.unnormalizedCoordinates = VK_FALSE};

	if (const VkResult result = vkCreateSampler(vkc.getLogicalDevice(), &samplerInfo, nullptr, &data.textureSampler);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Texture: Error creating texture sampler ({}).", internal::resultString(result))
	}
}

uint64_t Texture2D::getRendererId() const {
	auto &desc = internal::Descriptors::get();
	auto &texData = desc.getTextureData(m_textureId);
	if (texData.textureDescriptorSet == nullptr)
		texData.createDescriptorSet();
	return reinterpret_cast<uint64_t>(texData.textureDescriptorSet);
}

}// namespace owl::renderer::vulkan
