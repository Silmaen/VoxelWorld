/**
 * @file Buffer.h
 * @author Silmen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <utility>

#include "core/Core.h"
namespace owl::renderer {

/**
 * @brief Type of data
 */
enum class ShaderDataType {
	None = 0,
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
	Int,
	Int2,
	Int3,
	Int4,
	Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float:
			return 4;
		case ShaderDataType::Float2:
			return 4 * 2;
		case ShaderDataType::Float3:
			return 4 * 3;
		case ShaderDataType::Float4:
			return 4 * 4;
		case ShaderDataType::Mat3:
			return 4 * 3 * 3;
		case ShaderDataType::Mat4:
			return 4 * 4 * 4;
		case ShaderDataType::Int:
			return 4;
		case ShaderDataType::Int2:
			return 4 * 2;
		case ShaderDataType::Int3:
			return 4 * 3;
		case ShaderDataType::Int4:
			return 4 * 4;
		case ShaderDataType::Bool:
			return 1;
		case ShaderDataType::None:
			return 0;
	}
	OWL_CORE_ASSERT(false, "Unknown ShaderDataType!")
	return 0;
}

/**
 * @brief Class BufferElement
 */
struct OWL_API BufferElement {
	/// Element Name
	std::string name;
	/// Data's type
	ShaderDataType type;
	/// Data's size
	uint32_t size = 0;
	/// Data's offset
	uint32_t offset = 0;
	/// If data's normalized
	bool normalized = false;
	/**
	 * @brief Constructor
	 * @param name_ Element Name
	 * @param type_ Data's type
	 * @param normalized_ If data's normalized
	 */
	BufferElement(std::string&& name_, ShaderDataType type_, bool normalized_ = false)
		: name(std::move(name_)), type(type_), size(ShaderDataTypeSize(type_)), offset(0), normalized(normalized_) {
	}
	/**
	 * @brief Get component's count
	 * @return Component's count
	 */
	[[nodiscard]] uint32_t getComponentCount() const {
		switch (type) {
			case ShaderDataType::Float:
				return 1;
			case ShaderDataType::Float2:
				return 2;
			case ShaderDataType::Float3:
				return 3;
			case ShaderDataType::Float4:
				return 4;
			case ShaderDataType::Mat3:
				return 3 * 3;
			case ShaderDataType::Mat4:
				return 4 * 4;
			case ShaderDataType::Int:
				return 1;
			case ShaderDataType::Int2:
				return 2;
			case ShaderDataType::Int3:
				return 3;
			case ShaderDataType::Int4:
				return 4;
			case ShaderDataType::Bool:
				return 1;
			case ShaderDataType::None:
				return 0;
		}
		OWL_CORE_ASSERT(false, "Unknown ShaderDataType!")
		return 0;
	}
};

/**
 * @brief Class BufferLayout
 */
class OWL_API BufferLayout {
public:
	using element_type = std::vector<BufferElement>;
	using iterator = element_type::iterator;
	using const_iterator = element_type::const_iterator;
	/**
	 * @brief Constructor
	 * @param elements_ Elements in the layout
	 */
	BufferLayout(const std::initializer_list<BufferElement> &elements_)
		: elements(elements_) {
		CalculateOffsetsAndStride();
	}
	/**
	 * @brief Get buffer stride
	 * @return The buffer stride
	 */
	[[nodiscard]] uint32_t getStride() const { return stride; }
	/**
	 * @brief Get the buffer Elements
	 * @return Buffer elements
	 */
	[[nodiscard]] const std::vector<BufferElement> &getElements() const { return elements; }

	[[nodiscard]] iterator begin() { return elements.begin(); }
	[[nodiscard]] iterator end() { return elements.end(); }
	[[nodiscard]] const_iterator begin() const { return elements.begin(); }
	[[nodiscard]] const_iterator end() const { return elements.end(); }

private:
	/// List of element in the buffer data
	element_type elements;
	/// Stride of the data
	uint32_t stride = 0;
	/**
	 * @brief Automate computation of the offsets and stride
	 */
	void CalculateOffsetsAndStride() {
		uint32_t offset = 0;
		stride = 0;
		for (auto &element: elements) {
			element.offset = offset;
			offset += element.size;
			stride += element.size;
		}
	}
};


#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class VertexBuffer
 */
class OWL_API VertexBuffer {
public:
	VertexBuffer(const VertexBuffer &) = default;
	VertexBuffer(VertexBuffer &&) = default;
	VertexBuffer &operator=(const VertexBuffer &) = default;
	VertexBuffer &operator=(VertexBuffer &&) = default;
	VertexBuffer() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~VertexBuffer() = default;//---UNCOVER---
	/**
	 * @brief Activate the buffer in the GPU
	 */
	virtual void bind() const = 0;

	/**
	 * @brief Deactivate the buffer in the GPU
	 */
	virtual void unbind() const = 0;

	/**
	 * @brief Defines the data of the vertex buffer
	 * @param data The raw data
	 * @param size Number of data
	 */
	virtual void setData(const void* data, uint32_t size) = 0;
	/**
	 * @brief Create a new empty Vertex buffer in the GPU memory
	 * @param size Amount of data to read
	 * @return Pointer to the created buffer
	 */
	static shrd<VertexBuffer> create( uint32_t size);
	/**
	 * @brief Create a new Vertex buffer in the GPU memory
	 * @param vertices List of vertices data
	 * @param size Amount of data to read
	 * @return Pointer to the created buffer
	 */
	static shrd<VertexBuffer> create(float *vertices, uint32_t size);


	/**
	 * @brief Get the buffer data layout
	 * @return Data layout
	 */
	[[nodiscard]] const BufferLayout &getLayout() const { return layout; }

	/**
	 * @brief Define the data layout
	 * @param layout_ New data layout
	 */
	void setLayout(const BufferLayout &layout_) { layout = layout_; }

private:
	/// Data layout description
	BufferLayout layout{};
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class IndexBuffer
 */
class OWL_API IndexBuffer {
public:
	IndexBuffer(const IndexBuffer &) = default;
	IndexBuffer(IndexBuffer &&) = default;
	IndexBuffer &operator=(const IndexBuffer &) = default;
	IndexBuffer &operator=(IndexBuffer &&) = default;
	/**
	 * @brief Default constructor.
	 */
	IndexBuffer() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~IndexBuffer() = default;//---UNCOVER---
	/**
	 * @brief Activate the buffer in the GPU
	 */
	virtual void bind() const = 0;

	/**
	 * @brief Deactivate the buffer in the GPU
	 */
	virtual void unbind() const = 0;

	/**
	 * @brief Get the number of element in the buffer
	 * @return Number of element in the buffer
	 */
	[[nodiscard]] virtual uint32_t getCount() const = 0;
	/**
	 * @brief Create a new Index buffer in the GPU memory
	 * @param indices List of indices data
	 * @param count Amount of data to read
	 * @return Pointer to the created buffer
	 */
	static shrd<IndexBuffer> create(uint32_t *indices, uint32_t count);

private:
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
