/**
 * @file UniformBuffer.h
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "../UniformBuffer.h"

namespace owl::renderer::vulkan {

/**
 * @brief Class UniformBuffer.
 */
class UniformBuffer : public ::owl::renderer::UniformBuffer {
public:
	UniformBuffer() = delete;
	UniformBuffer(const UniformBuffer &) = default;
	UniformBuffer(UniformBuffer &&) = default;

	/**
	 * @brief Constructor.
	 * @param size Buffer's size.
	 * @param binding Buffer's binding.
	 */
	UniformBuffer(uint32_t size, uint32_t binding);

	/**
	 * @brief Destructor.
	 */
	~UniformBuffer() override;

	/**
	 * @brief bind this uniform buffer.
	 */
	void bind() override {}

	/**
	 * @brief Push Data to GPU.
	 * @param data The data.
	 * @param size The data size.
	 * @param offset The offset to start.
	 */
	void setData(const void *data, uint32_t size, uint32_t offset) override;

private:
};

}// namespace owl::renderer::vulkan
