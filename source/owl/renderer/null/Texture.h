/**
 * @file Texture.h
 * @author Silmaen
 * @date 30/07/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "renderer/Texture.h"

namespace owl::renderer::null {
/**
 * @brief Class Texture.
 */
class OWL_API Texture2D final : public ::owl::renderer::Texture2D {
public:
	Texture2D(const Texture2D &) = default;
	Texture2D(Texture2D &&) = default;
	Texture2D &operator=(const Texture2D &) = default;
	Texture2D &operator=(Texture2D &&) = default;

	/**
	 * @brief Default constructor.
	 * @param[in] iPath path to the texture image file.
	 */
	explicit Texture2D(std::filesystem::path iPath);
	/**
	 * @brief Constructor by size.
	 * @param[in] iWidth Texture's width.
	 * @param[in] iHeight Texture's height.
	 * @param[in] iWithAlpha Texture has alpha channel.
	 */
	Texture2D(uint32_t iWidth, uint32_t iHeight, bool iWithAlpha = true);
	/**
	 * @brief Constructor by size.
	 * @param[in] iSize Texture's width.
	 * @param[in] iWithAlpha Texture has alpha channel.
	 */
	explicit Texture2D(math::vec2ui iSize, bool iWithAlpha = true);

	/**
	 * @brief Destructor.
	 */
	~Texture2D() override;

	/**
	 * @brief Comparison operator.
	 * @param[in] iOther Other texture to compare.
	 * @return True if same.
	 */
	bool operator==(const Texture &iOther) const override {
		return m_rendererId == (dynamic_cast<const Texture2D &>(iOther)).m_rendererId;
	}

	/**
	 * @brief Get renderer id.
	 * @return The renderer ID.
	 */
	[[nodiscard]] uint64_t getRendererId() const override { return m_rendererId; }

	/**
	 * @brief Activate the texture in the GPU.
	 * @param[in] iSlot Slot into put the texture.
	 */
	void bind(uint32_t iSlot) const override;

	/**
	 * @brief Define the texture data.
	 * @param[in] iData Raw data.
	 * @param[in] iSize Size of the data.
	 */
	void setData(void *iData, uint32_t iSize) override;

private:
	/// OpenGL binding.
	uint64_t m_rendererId = 0;
};
}// namespace owl::renderer::null
