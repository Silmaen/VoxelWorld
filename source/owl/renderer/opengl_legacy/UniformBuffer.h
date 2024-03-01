/**
 * @file UniformBuffer.h
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "../UniformBuffer.h"

namespace owl::renderer::opengl_legacy {
/**
 * @brief Class UniformBuffer.
 */
class UniformBuffer final : public ::owl::renderer::UniformBuffer {
public:
	UniformBuffer() = delete;
	UniformBuffer(const UniformBuffer&) = default;
	UniformBuffer(UniformBuffer&&) = default;
	UniformBuffer& operator=(const UniformBuffer&) = default;
	UniformBuffer& operator=(UniformBuffer&&) = default;

	/**
	 * @brief Constructor.
	 * @param[in] iSize Buffer's size.
	 * @param[in] iBinding Buffer's binding.
	 */
	UniformBuffer(uint32_t iSize, uint32_t iBinding);
	/**
	 * @brief Destructor.
	 */
	~UniformBuffer() override;

	/**
	 * @brief bind this uniform buffer.
	 */
	void bind() override {}

	/**
	 * @brief Push Data to GPU.
	 * @param[in] iData The data.
	 * @param[in] iSize The data size.
	 * @param[in] iOffset The offset to start.
	 */
	void setData(const void* iData, uint32_t iSize, uint32_t iOffset) override;

	[[nodiscard]] uint32_t getBinding() const { return m_internalBinding; }

	[[nodiscard]] const char* getData() const { return m_internalData.data(); }

private:
	/// The binding number.
	uint32_t m_internalBinding = 0;
	std::vector<char> m_internalData;
};
} // namespace owl::renderer::opengl_legacy
