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

UniformBuffer::UniformBuffer(uint32_t size, uint32_t, const std::string &) {
	auto &vkh = internal::VulkanHandler::get();
	vkh.createUniformBuffers(size);
}

UniformBuffer::~UniformBuffer() = default;

void UniformBuffer::setData(const void *data, uint32_t size, uint32_t) {
	const auto &vkh = internal::VulkanHandler::get();
	vkh.setUniformData(data, size);
}


}// namespace owl::renderer::vulkan
