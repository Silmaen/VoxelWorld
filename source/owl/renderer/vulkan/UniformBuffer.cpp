/**
 * @file UniformBuffer.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "UniformBuffer.h"

#include "internal/VulkanHandler.h"

namespace owl::renderer::vulkan {

UniformBuffer::UniformBuffer(const uint32_t iSize, uint32_t, const std::string &) {
	auto &vkh = internal::VulkanHandler::get();
	vkh.createUniformBuffers(iSize);
}

UniformBuffer::~UniformBuffer() = default;

void UniformBuffer::setData(const void *iData, const uint32_t iSize, uint32_t) {
	const auto &vkh = internal::VulkanHandler::get();
	vkh.setUniformData(iData, iSize);
}


}// namespace owl::renderer::vulkan
