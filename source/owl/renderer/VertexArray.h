/**
 * @file VertexArray.h
 * @author Silmen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Buffer.h"
#include "core/Core.h"

namespace owl::renderer {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class VertexArray
 */
class OWL_API VertexArray {
public:
	using vertexBuf = shrd<VertexBuffer>;
	using indexBuf = shrd<IndexBuffer>;
	using vertBufs = std::vector<vertexBuf>;

	VertexArray(const VertexArray &) = default;
	VertexArray(VertexArray &&) = default;
	VertexArray &operator=(const VertexArray &) = default;
	VertexArray &operator=(VertexArray &&) = default;
	/**
	 * @brief Default constructor.
	 */
	VertexArray() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~VertexArray() = default;//---UNCOVER---

	/**
	 * @brief Activate the buffer in the GPU
	 */
	virtual void bind() const = 0;
	/**
	 * @brief Deactivate the buffer in the GPU
	 */
	virtual void unbind() const = 0;

	/**
	 * @brief Append a vertex buffer to the list
	 * @param vertexBuffer Vertex buffer to add
	 */
	virtual void addVertexBuffer(const vertexBuf& vertexBuffer) = 0;
	/**
	 * @brief Define the Index buffer
	 * @param indexBuffer New Index Buffer
	 */
	virtual void setIndexBuffer(const indexBuf& indexBuffer) = 0;

	/**
	 * @brief Access to vertex buffers
	 * @return Vertex buffers
	 */
	[[nodiscard]] virtual const vertBufs& getVertexBuffers() const = 0;
	/**
	 * @brief Access to index buffer
	 * @return Index buffer
	 */
	[[nodiscard]] virtual const indexBuf& getIndexBuffer() const = 0;

	/**
	 * @brief Static method to create a Vertex Array
	 * @return Vertex Array
	 */
	static shrd<VertexArray> create();
private:
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
