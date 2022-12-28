/**
 * @file UniformBuffer.h
 * @author Silmaen
 * @date 02/01/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "../UniformBuffer.h"

namespace owl::renderer::opengl {

/**
 * @brief Class UniformBuffer
 */
class UniformBuffer: public ::owl::renderer::UniformBuffer {
public:
	/**
	 * @brief Default copy constructor
	 */
	UniformBuffer(const UniformBuffer &) = default;
	/**
	 * @brief Default move constructor
	 */
	UniformBuffer(UniformBuffer &&) = default;
	/**
	 * @brief Default copy assignation
	 * @return this
	 */
	UniformBuffer &operator=(const UniformBuffer &) = default;
	/**
	 * @brief Default move assignation
	 * @return this
	 */
	UniformBuffer &operator=(UniformBuffer &&) = default;
	/**
	 * @brief Default constructor.
	 */
	UniformBuffer() = delete;
	/**
	 * @brief Constructor
	 * @param size Buffer's size
	 * @param binding Buffer's binding
	 */
	UniformBuffer(uint32_t size, uint32_t binding);
	/**
	 * @brief Destructor.
	 */
	~UniformBuffer() override;
	/**
	 * @brief Push Data to GPU
	 * @param data The data
	 * @param size The data size
	 * @param offset The offset to start
	 */
	void setData(const void* data, uint32_t size, uint32_t offset = 0) override;
private:
	/// The renderer's ID
	uint32_t rendererId =0;
};

}// namespace owl::renderer::opengl
