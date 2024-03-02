/**
 * @file Texture.h
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "renderer/Texture.h"

namespace owl::renderer::opengl_legacy {
/**
 * @brief Class Texture.
 */
class Texture2D final : public renderer::Texture2D {
public:
	Texture2D(const Texture2D&) = default;
	Texture2D(Texture2D&&) = default;
	Texture2D& operator=(const Texture2D&) = default;
	Texture2D& operator=(Texture2D&&) = default;

	/**
	 * @brief Default constructor.
	 * @param[in] iPath path to the texture image file.
	 */
	explicit Texture2D(std::filesystem::path iPath);
	/**
	 * @brief Constructor by size.
	 * @param[in] iWidth Texture's width.
	 * @param[in] iHeight Texture's height.
	 * @param[in] iWithAlpha If the texture has alpha channel.
	 */
	Texture2D(uint32_t iWidth, uint32_t iHeight, bool iWithAlpha = true);
	/**
	 * @brief Constructor by size.
	 * @param[in] iSize Texture's width.
	 * @param[in] iWithAlpha If the texture has alpha channel.
	 */
	explicit Texture2D(math::FrameSize iSize, bool iWithAlpha = true);

	/**
	 * @brief Destructor.
	 */
	~Texture2D() override;

	/**
	 * @brief Comparison operator.
	 * @param[in] iOther Other texture to compare.
	 * @return True if same.
	 */
	bool operator==(const Texture& iOther) const override {
		return m_textureId == (dynamic_cast<const Texture2D&>(iOther)).m_textureId;
	}

	/**
	 * @brief Access to texture's width.
	 * @return Texture's width.
	 */
	[[nodiscard]] uint32_t getWidth() const override { return m_size.getWidth(); }

	/**
	 * @brief Access to texture's height.
	 * @return Texture's height.
	 */
	[[nodiscard]] uint32_t getHeight() const override { return m_size.getHeight(); }

	/**
	 * @brief Access to texture's size.
	 * @return Texture's size.
	 */
	[[nodiscard]] math::FrameSize getSize() const override { return m_size; }

	/**
	 * @brief Tells if the data effectively loaded.
	 * @return True if texture contains data.
	 */
	[[nodiscard]] bool isLoaded() const override { return m_size.surface() > 0; }

	/**
	 * @brief Get renderer id.
	 * @return The renderer ID.
	 */
	[[nodiscard]] uint32_t getRendererId() const override { return m_textureId; }

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
	void setData(void* iData, uint32_t iSize) override;

	/**
	 * @brief Get Path to texture file.
	 * @return Path to texture file.
	 */
	[[nodiscard]] const std::filesystem::path& getPath() const override {
		return m_path;
	}

private:
	/// Path to the texture file.
	std::filesystem::path m_path;
	/// Texture's size.
	math::FrameSize m_size = {0, 0};
	/// OpenGL binding.
	uint32_t m_textureId = 0;
	/// If the image has Alpha channel.
	bool m_hasAlpha = true;
};
} // namespace owl::renderer::opengl_legacy
