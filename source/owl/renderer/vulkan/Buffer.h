/**
 * @file Buffer.h
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../Buffer.h"
#include <vulkan/vulkan.h>

namespace owl::renderer::vulkan {

/**
 * @brief Class VertexBuffer.
 */
class VertexBuffer final : public ::owl::renderer::VertexBuffer {
public:
	VertexBuffer(const VertexBuffer &) = delete;
	VertexBuffer(VertexBuffer &&) = delete;
	VertexBuffer &operator=(const VertexBuffer &) = delete;
	VertexBuffer &operator=(VertexBuffer &&) = delete;
	/**
	 * @brief Constructor.
	 * @param size The buffer size.
	 */
	explicit VertexBuffer(uint32_t size);

	/**
	 * @brief Default constructor.
	 */
	VertexBuffer(const float *vertices, const uint32_t size);

	/**
	 * @brief Destructor.
	 */
	~VertexBuffer() override;

	/**
	 * @brief Release the memory buffer
	 */
	void release();

	/**
	 * @brief Activate the buffer in the GPU.
	 */
	void bind() const override;

	/**
	 * @brief Deactivate the buffer in the GPU.
	 */
	void unbind() const override;

	/**
	 * @brief Defines the data of the vertex buffer.
	 * @param data The raw data.
	 * @param size Number of data.
	 */
	void setData(const void *data, const uint32_t size) override;

	/**
	 * @brief Get the binding description.
	 * @return The bining description.
	 */
	[[nodiscard]] VkVertexInputBindingDescription getBindingDescription() const;

	/**
	 * @brief Get The attribute desciption.
	 * @return The attribute description.
	 */
	[[nodiscard]] std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() const;

private:
	/// The vulkan vertex buffer.
	VkBuffer vertexBuffer{nullptr};
	/// The vulkan vertex buffer memory.
	VkDeviceMemory vertexBufferMemory{nullptr};

	void createBuffer(const float *data, const uint32_t size);
};

/**
 * @brief Class IndexBuffer.
 */
class IndexBuffer final : public ::owl::renderer::IndexBuffer {
public:
	IndexBuffer(const IndexBuffer &) = delete;
	IndexBuffer(IndexBuffer &&) = delete;
	IndexBuffer &operator=(const IndexBuffer &) = delete;
	IndexBuffer &operator=(IndexBuffer &&) = delete;
	/**
	 * @brief Default constructor.
	 * @param indices Array of indices.
	 * @param size Number of indices in the array.
	 */
	IndexBuffer(uint32_t *indices, uint32_t size);

	/**
	 * @brief Destructor.
	 */
	~IndexBuffer() override;

	/**
	 * @brief Release the memory buffer
	 */
	void release();

	/**
	 * @brief Activate the buffer in the GPU.
	 */
	void bind() const override;

	/**
	 * @brief Deactivate the buffer in the GPU.
	 */
	void unbind() const override;

	/**
	 * @brief Get the number of element in the buffer.
	 * @return Number of element in the buffer.
	 */
	[[nodiscard]] uint32_t getCount() const override { return count; }

private:
	/// Number of elements.
	uint32_t count = 0;
	/// Vulkan index buffer.
	VkBuffer indexBuffer{nullptr};
	/// Vulkan memory buffer.
	VkDeviceMemory indexBufferMemory{nullptr};
};

}// namespace owl::renderer::vulkan
