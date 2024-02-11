/**
 * @file Texture.h
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "renderer/Texture.h"

namespace owl::renderer::vulkan {
/**
 * @brief Class Texture.
 */
class Texture2D final : public ::owl::renderer::Texture2D {
public:
	Texture2D(const Texture2D &) = default;
	Texture2D(Texture2D &&) = default;
	Texture2D &operator=(const Texture2D &) = default;
	Texture2D &operator=(Texture2D &&) = default;

	/**
	 * @brief Default constructor.
	 * @param path_ path to the texture image file.
	 */
	explicit Texture2D(std::filesystem::path path_);
	/**
	 * @brief Constructor by size.
	 * @param width_ Texture's width.
	 * @param height_ Texture's height.
	 * @param withAlpha If the texture has alpha channel.
	 */
	Texture2D(uint32_t width_, uint32_t height_, bool withAlpha = true);
	/**
	 * @brief Constructor by size.
	 * @param size Texture's width.
	 * @param withAlpha If the texture has alpha channel.
	 */
	explicit Texture2D(const math::FrameSize &size, bool withAlpha = true);

	/**
	 * @brief Destructor.
	 */
	~Texture2D() override;

	/**
	 * @brief Comparison operator.
	 * @param other Other texture to compare.
	 * @return True if same.
	 */
	bool operator==(const Texture &other) const override {
		return rendererID == (dynamic_cast<const Texture2D &>(other)).rendererID;
	}

	/**
	 * @brief Access to texture's width.
	 * @return Texture's width.
	 */
	[[nodiscard]] uint32_t getWidth() const override { return size.getWidth(); }

	/**
	 * @brief Access to texture's height.
	 * @return Texture's height.
	 */
	[[nodiscard]] uint32_t getHeight() const override { return size.getHeight(); }

	/**
	 * @brief Access to texture's size.
	 * @return Texture's size.
	 */
	[[nodiscard]] math::FrameSize getSize() const override { return size; }

	/**
	 * @brief Tells if the data effectively loaded.
	 * @return True if texture contains data.
	 */
	[[nodiscard]] bool isLoaded() const override { return size.surface() > 0; }

	/**
	 * @brief Get renderer id.
	 * @return The renderer ID.
	 */
	[[nodiscard]] uint32_t getRendererID() const override { return rendererID; }

	/**
	 * @brief Activate the texture in the GPU.
	 * @param slot Slot into put the texture.
	 */
	void bind(uint32_t slot) const override;

	/**
	 * @brief Define the texture data.
	 * @param data Raw data.
	 * @param size Size of the data.
	 */
	void setData(void *data, uint32_t size) override;

	/**
	 * @brief Get Path to texture file.
	 * @return Path to texture file.
	 */
	[[nodiscard]] const std::filesystem::path &getPath() const override {
		return path;
	}

private:
	/// Path to the texture file.
	std::filesystem::path path;
	/// Texture's size.
	math::FrameSize size = {0, 0};
	/// OpenGL binding.
	uint32_t rendererID = 0;
};
}// namespace owl::renderer::vulkan
