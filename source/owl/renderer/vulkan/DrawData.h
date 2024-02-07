/**
 * @file DrawData.h
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "../DrawData.h"
#include "Shader.h"

namespace owl::renderer::vulkan {

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
	 * @param renderer Name of the shader's related renderer.
	 * @param indices List of vertex indices.
	 * @param shaderName The shader name.
	 */
	void init(const BufferLayout &layout_, const std::string &renderer, std::vector<uint32_t> &indices, const std::string &shaderName) override;

	/**
	 * @brief Bind this draw data.
	 */
	void bind() const override;

	/**
	 * @brief Unbind the draw data.
	 */
	void unbind() const override {}

	/**
	 * @brief Push Vertices data  to the draw buffer.
	 * @param data The raw vertices data
	 * @param size The size of the raw data.
	 */
	void setVertexData([[maybe_unused]] const void *data, [[maybe_unused]] uint32_t size) override {}

	/**
	 * @brief Get the number of vertex to draw.
	 * @return Number of vertex to draw
	 */
	[[nodiscard]] uint32_t getIndexCount() const override { return 0; }

	/**
	 * @brief Define the shader for this object.
	 * @param shaderName The shader name.
	 * @param renderer Name of the shader's related renderer.
	 */
	void setShader(const std::string &shaderName, const std::string &renderer) override;


private:
	/// Pointer to the shader/pipeline.
	shared<Shader> shader;
};

}// namespace owl::renderer::vulkan
