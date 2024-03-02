/**
 * @file UniformBuffer.h
 * @author Silmaen
 * @date 02/01/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

namespace owl::renderer {

/**
 * @brief Class UniformBuffer.
 */
class OWL_API UniformBuffer {
public:
	UniformBuffer() = default;
	UniformBuffer(const UniformBuffer &) = default;
	UniformBuffer(UniformBuffer &&) = default;
	UniformBuffer &operator=(const UniformBuffer &) = default;
	UniformBuffer &operator=(UniformBuffer &&) = default;

	/**
	 * @brief Destructor.
	 */
	virtual ~UniformBuffer();

	/**
	 * @brief Push Data to GPU.
	 * @param[in] iData The data.
	 * @param[in] iSize The data size.
	 * @param[in] iOffset The offset to start.
	 */
	virtual void setData(const void *iData, uint32_t iSize, uint32_t iOffset) = 0;

	/**
	 * @brief bind this uniform buffer.
	 */
	virtual void bind() = 0;

	/**
	 * @brief Create a new instance of UniformBuffer.
	 * @param[in] iSize The buffer size.
	 * @param[in] iBinding The binding.
	 * @param[in] iRenderer Name of the shader's related renderer.
	 * @return New instance of UniformBuffer.
	 */
	static shared<UniformBuffer> create(uint32_t iSize, uint32_t iBinding, const std::string &iRenderer);
};

}// namespace owl::renderer
