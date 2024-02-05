/**
 * @file DrawData.h
 * @author Silmaen
 * @date 04/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "Buffer.h"

namespace owl::renderer {

/**
 * @brief Abstract class representing what is required for a draw.
 */
class OWL_API DrawData {
public:
	DrawData(const DrawData &) = default;
	DrawData(DrawData &&) = default;
	DrawData &operator=(const DrawData &) = default;
	DrawData &operator=(DrawData &&) = default;
	DrawData() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~DrawData();

	/**
	 * @brief Initialize the draw data.
	 * @param layout_ Layout of the vertex attributes.
	 * @param renderer Name of the shader's related renderer.
	 * @param indices List of vertex indices.
	 * @param shaderName The shader name.
	 */
	virtual void init(const BufferLayout &layout_, const std::string &renderer, std::vector<uint32_t> &indices, const std::string &shaderName) = 0;

	/**
	 * @brief Bind this draw data.
	 */
	virtual void bind() const = 0;

	/**
	 * @brief Unbind the draw data.
	 */
	virtual void unbind() const = 0;

	/**
	 * @brief Push Vertices data  to the draw buffer.
	 * @param data The raw vertices data
	 * @param size The size of the raw data.
	 */
	virtual void setVertexData(const void *data, uint32_t size) = 0;

	/**
	 * @brief Get the number of vertex to draw.
	 * @return Number of vertex to draw
	 */
	[[nodiscard]] virtual uint32_t getIndexCount() const = 0;

	/**
	 * @brief Create a new drax data buffer.
	 * @return Pointer to the created buffer.
	 */
	static shared<DrawData> create();

	/**
	 * @brief Define the shader for this object.
	 * @param shaderName The shader name.
	 * @param renderer Name of the shader's related renderer.
	 */
	virtual void setShader(const std::string &shaderName, const std::string &renderer) = 0;
};

}// namespace owl::renderer
