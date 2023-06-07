/**
 * @file Buffer.h
 * @author Silmaen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../opengl/Buffer.h"

namespace owl::renderer::opengl_legacy {

/**
 * @brief Class VertexBuffer
 */
class VertexBuffer : public ::owl::renderer::VertexBuffer {
public:
	VertexBuffer(const VertexBuffer &) = delete;
	VertexBuffer(VertexBuffer &&) = delete;
	VertexBuffer &operator=(const VertexBuffer &) = delete;
	VertexBuffer &operator=(VertexBuffer &&) = delete;
	/**
	 * @brief Default constructor.
	 * @param size The buffer size.
	 */
	VertexBuffer(uint32_t size);

	/**
	 * @brief Default constructor.
	 */
	VertexBuffer(float *vertices, uint32_t size);

	/**
	 * @brief Destructor.
	 */
	~VertexBuffer() override;//---UNCOVER---

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
	void setData(const void *data, uint32_t size) override;

private:
	/// Internal buffer data.
	std::vector<float> data;
};

/**
 * @brief Class IndexBuffer.
 */
class IndexBuffer : public ::owl::renderer::IndexBuffer {
public:
	IndexBuffer(const IndexBuffer &) = delete;
	IndexBuffer(IndexBuffer &&) = delete;
	IndexBuffer &operator=(const IndexBuffer &) = delete;
	IndexBuffer &operator=(IndexBuffer &&) = delete;
	/**
	 * @brief Default constructor.
	 * @param indices Array of indices.
	 * @param count Number of indices in the array.
	 */
	IndexBuffer(uint32_t *indices, uint32_t count);

	/**
	 * @brief Destructor.
	 */
	~IndexBuffer() override;//---UNCOVER---

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
	/// Internal buffer data.
	std::vector<uint32_t> data;
	/// Number of elements.
	uint32_t count = 0;
};

}// namespace owl::renderer::opengl_legacy
