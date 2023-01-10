/**
 * @file UniformBuffer.h
 * @author Silmaen
 * @date 02/01/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

namespace owl::renderer {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class UniformBuffer
 */
class UniformBuffer {
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
	UniformBuffer() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~UniformBuffer() = default;
	/**
	 * @brief Push Data to GPU
	 * @param data The data
	 * @param size The data size
	 * @param offset The offset to start
	 */
	virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
	/**
	 * @brief Create a new instance of UniformBuffer
	 * @param size The buffer size
	 * @param binding The binding
	 * @return New instance of UniformBuffer
	 */
	static shrd<UniformBuffer> create(uint32_t size, uint32_t binding);

};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
