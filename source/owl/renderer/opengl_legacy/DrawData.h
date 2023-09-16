/**
 * @file DrawData.h
 * @author Silmaen
 * @date 05/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "../DrawData.h"
#include "Buffer.h"
#include "Shader.h"

namespace owl::renderer::opengl_legacy {

/**
 * @brief Class representing what is required for a draw.
 */
class OWL_API DrawData : public owl::renderer::DrawData {
public:
	DrawData(const DrawData &) = default;
	DrawData(DrawData &&) = default;
	DrawData &operator=(const DrawData &) = default;
	DrawData &operator=(DrawData &&) = default;
	DrawData() = default;
	/**
	 * @brief Destructor.
	 */
	~DrawData() override;

	/**
	 * @brief Initialize the draw data.
	 * @param layout_ Layout of the vertex attributes.
	 * @param indices List of vertex indices.
	 * @param shaderName The shader name.
	 */
	void init([[maybe_unused]] const BufferLayout &layout_, [[maybe_unused]] std::vector<uint32_t> &indices, [[maybe_unused]] const std::string &shaderName) override;

	/**
	 * @brief Bind this draw data.
	 */
	void bind() const override;

	/**
	 * @brief Unbind the draw data.
	 */
	void unbind() const override;

	/**
	 * @brief Push Vertices data  to the draw buffer.
	 * @param data The raw vertices data
	 * @param size The size of the raw data.
	 */
	void setVertexData([[maybe_unused]] const void *data, [[maybe_unused]] uint32_t size) override;

	/**
	 * @brief Get the number of vertex to draw.
	 * @return Number of vertex to draw
	 */
	[[nodiscard]] uint32_t getIndexCount() const override;

	/**
	 * @brief Define the shader for this object.
	 * @param shaderName The shader name.
	 */
	void setShader(const std::string &shaderName) override;

private:
	void applyLayout() const;
	void unApplyLayout() const;
	bool active = false;
	shared<VertexBuffer> vertexBuffer;
	shared<IndexBuffer> indexBuffer;
	shared<Shader> shader;
};

}// namespace owl::renderer::opengl_legacy