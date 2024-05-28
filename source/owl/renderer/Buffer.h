/**
 * @file Buffer.h
 * @author Silmaen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

namespace owl::renderer {
/**
 * @brief Type of data.
 */
enum class ShaderDataType : uint8_t {
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

static uint32_t shaderDataTypeSize(const ShaderDataType iType) {
	switch (iType) {
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
 * @brief Class BufferElement.
 */
struct OWL_API BufferElement {
	/// Element Name.
	std::string name;
	/// Data's type.
	ShaderDataType type;
	/// Data's size.
	uint32_t size{0};
	/// Data's offset.
	uint32_t offset{0};
	/// If data's normalized.
	bool normalized = false;

	/**
	 * @brief Constructor.
	 * @param[in] iName Element Name.
	 * @param[in] iType Data's type.
	 * @param[in] iNormalized If data's normalized.
	 */
	BufferElement(std::string &&iName, const ShaderDataType iType, const bool iNormalized = false)
		: name(std::move(iName)), type(iType), size(shaderDataTypeSize(iType)), normalized(iNormalized) {}

	/**
	 * @brief Get component's count.
	 * @return Component's count.
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
 * @brief Class BufferLayout.
 */
class OWL_API BufferLayout {
public:
	using element_type = std::vector<BufferElement>;
	using iterator = element_type::iterator;
	using const_iterator = element_type::const_iterator;

	BufferLayout() = default;
	/**
	 * @brief Constructor.
	 * @param[in] iElements Elements in the layout.
	 */
	BufferLayout(const std::initializer_list<BufferElement> &iElements) : m_elements(iElements) {
		calculateOffsetsAndStride();
	}

	/**
	 * @brief Get buffer stride.
	 * @return The buffer stride.
	 */
	[[nodiscard]] uint32_t getStride() const { return m_stride; }

	/**
	 * @brief Get the buffer Elements.
	 * @return Buffer elements.
	 */
	[[nodiscard]] const std::vector<BufferElement> &getElements() const { return m_elements; }

	[[nodiscard]] iterator begin() { return m_elements.begin(); }
	[[nodiscard]] iterator end() { return m_elements.end(); }
	[[nodiscard]] const_iterator begin() const { return m_elements.begin(); }
	[[nodiscard]] const_iterator end() const { return m_elements.end(); }

private:
	/// List of element in the buffer data.
	element_type m_elements;
	/// Stride of the data.
	uint32_t m_stride = 0;
	/**
	 * @brief Automate computation of the offsets and stride.
	 */
	void calculateOffsetsAndStride() {
		uint32_t offset = 0;
		m_stride = 0;
		for (auto &element: m_elements) {
			element.offset = offset;
			offset += element.size;
			m_stride += element.size;
		}
	}
};

/**
 * @brief Class VertexBuffer.
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
	virtual ~VertexBuffer();

	/**
	 * @brief Activate the buffer in the GPU.
	 */
	virtual void bind() const = 0;

	/**
	 * @brief Deactivate the buffer in the GPU.
	 */
	virtual void unbind() const = 0;

	/**
	 * @brief Defines the data of the vertex buffer.
	 * @param[in] iData The raw data.
	 * @param[in] iSize Number of data.
	 */
	virtual void setData(const void *iData, uint32_t iSize) = 0;

	/**
	 * @brief Get the buffer data layout.
	 * @return Data layout.
	 */
	[[nodiscard]] const BufferLayout &getLayout() const { return m_layout; }

	/**
	 * @brief Define the data layout.
	 * @param[in] iLayout New data layout.
	 */
	void setLayout(const BufferLayout &iLayout) { m_layout = iLayout; }

private:
	/// Data layout description.
	BufferLayout m_layout;
};

/**
 * @brief Class IndexBuffer.
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
	virtual ~IndexBuffer();
	/**
	 * @brief Activate the buffer in the GPU.
	 */
	virtual void bind() const = 0;

	/**
	 * @brief Deactivate the buffer in the GPU.
	 */
	virtual void unbind() const = 0;

	/**
	 * @brief Get the number of element in the buffer.
	 * @return Number of element in the buffer.
	 */
	[[nodiscard]] virtual uint32_t getCount() const = 0;
};
}// namespace owl::renderer
