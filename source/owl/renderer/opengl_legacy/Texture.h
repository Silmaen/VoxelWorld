/**
 * @file Texture.h
 * @author Silmaen
 * @date 30/06/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "renderer/Texture.h"

#include "gl_21/glad.h"

namespace owl::renderer::opengl_legacy {
/**
 * @brief Class Texture
 */
class Texture2D : public ::owl::renderer::Texture2D {
public:
	Texture2D(const Texture2D &) = default;
	Texture2D(Texture2D &&) = default;
	Texture2D &operator=(const Texture2D &) = default;
	Texture2D &operator=(Texture2D &&) = default;

	/**
	 * @brief Default constructor.
	 * @param path_ path to the texture image file
	 */
	explicit Texture2D(std::filesystem::path path_);

	/**
	 * @brief Constructor by size.
	 * @param width_ Texture's width.
	 * @param height_ Texture's height.
	 */
	Texture2D(uint32_t width_, uint32_t height_);

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
	 * @return Texture's width
	 */
	[[nodiscard]] uint32_t getWidth() const override { return width; }

	/**
	 * @brief Access to texture's height.
	 * @return Texture's height.
	 */
	[[nodiscard]] uint32_t getHeight() const override { return height; }

	/**
	 * @brief Tells if the data effectively loaded.
	 * @return True if texture contains data.
	 */
	[[nodiscard]] bool isLoaded() const override { return width * height > 0; }

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
	/// Texture's width.
	uint32_t width = 0;
	/// Texture's height
	uint32_t height = 0;
	/// OpenGL binding.
	uint32_t rendererID = 0;
	/// Texture internal format.
	uint32_t internalFormat;
	/// Texture data format.
	uint32_t dataFormat;
};
}// namespace owl::renderer::opengl_legacy
