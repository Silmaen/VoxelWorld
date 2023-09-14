/**
 * @file VertexArray.h
 * @author Silmaen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Buffer.h"

namespace owl::renderer::opengl {

/**
 * @brief Class VertexArray.
 */
class VertexArray {
public:
	VertexArray();
	VertexArray(const VertexArray &) = default;
	VertexArray(VertexArray &&) = default;
	VertexArray &operator=(const VertexArray &) = default;
	VertexArray &operator=(VertexArray &&) = default;

	using vertexBuf = shared<VertexBuffer>;
	using vertBufs = std::vector<vertexBuf>;
	using indexBuf = shared<IndexBuffer>;

	/**
	 * @brief Destructor.
	 */
	~VertexArray();

	/**
	 * @brief Activate the buffer in the GPU.
	 */
	void bind() const;
	/**
	 * @brief Deactivate the buffer in the GPU.
	 */
	void unbind() const;

	/**
	 * @brief Append a vertex buffer to the list.
	 * @param vertexBuffer Vertex buffer to add.
	 */
	void addVertexBuffer(const vertexBuf &vertexBuffer);

	/**
	 * @brief Define the Index buffer.
	 * @param indexBuffer New Index Buffer.
	 */
	void setIndexBuffer(const indexBuf &indexBuffer);

	/**
	 * @brief Access to vertex buffers.
	 * @return Vertex buffers.
	 */
	[[nodiscard]] const vertBufs &getVertexBuffers() const { return vertexBuffers; }

	/**
	 * @brief Access to index buffer.
	 * @return Index buffer.
	 */
	[[nodiscard]] const indexBuf &getIndexBuffer() const { return indexBuffer; }

private:
	/// Id in the OpenGL context.
	uint32_t rendererID = 0;
	/// Offset on index.
	uint32_t vertexBufferIndex = 0;
	/// The vertex buffers.
	vertBufs vertexBuffers;
	/// The index buffer.
	indexBuf indexBuffer;
};

}// namespace owl::renderer::opengl
