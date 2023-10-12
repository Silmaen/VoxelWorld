/**
 * @file Texture.h
 * @author Silmaen
 * @date 12/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/Core.h"
#include <filesystem>

namespace owl::renderer {

class TextureLibrary;
/**
 * @brief Class Texture.
 */
class OWL_API Texture {
public:
	Texture() = default;
	Texture(const Texture &) = default;
	Texture(Texture &&) = default;
	Texture &operator=(const Texture &) = default;
	Texture &operator=(Texture &&) = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~Texture();

	/**
	 * @brief Comparison operator.
	 * @param other Other texture to compare.
	 * @return True if same.
	 */
	virtual bool operator==(const Texture &other) const = 0;

	/**
	 * @brief Access to texture's width.
	 * @return Texture's width.
	 */
	[[nodiscard]] virtual uint32_t getWidth() const = 0;

	/**
	 * @brief Access to texture's height.
	 * @return Texture's height.
	 */
	[[nodiscard]] virtual uint32_t getHeight() const = 0;

	/**
	 * @brief Tells if the data effectively loaded.
	 * @return True if texture contains data.
	 */
	[[nodiscard]] virtual bool isLoaded() const = 0;

	/**
	 * @brief Get renderer id.
	 * @return The renderer ID.
	 */
	[[nodiscard]] virtual uint32_t getRendererID() const = 0;

	/**
	 * @brief Activate the texture in the GPU.
	 * @param slot Slot into put the texture.
	 */
	virtual void bind(uint32_t slot = 0) const = 0;

	/**
	 * @brief Get Path to texture file.
	 * @return Path to texture file.
	 */
	[[nodiscard]] virtual const std::filesystem::path &getPath() const = 0;

	/**
	 * @brief Define the texture data.
	 * @param data Raw data.
	 * @param size Size of the data.
	 */
	virtual void setData(void *data, uint32_t size) = 0;

	/**
	 * @brief Get access to the texture's name.
	 * @return The texture's name.
	 */
	[[nodiscard]] const std::string &getName() const { return name; }

private:
	/// The texture's name.
	std::string name;
	/// Library is a friend to be able to modify name.
	friend class TextureLibrary;
};


#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class texture 2D
 */
class OWL_API Texture2D : public Texture {
public:
	/**
	 * @brief Creates the texture with the given filename.
	 * @param file The path to the file to load.
	 * @return Resulting texture.
	 */
	static shared<Texture2D> create(const std::filesystem::path &file);

	/**
	 * @brief Create a new texture.
	 * @param textureName Name of the files in the standard path.
	 * @return Pointer to the texture.
	 */
	static shared<Texture2D> create(const std::string &textureName);
	/**
	 * @brief Creates the texture with the given size.
	 * @param width The texture's width.
	 * @param height The texture's height.
	 * @param withAlpha If the texture has alpha channel.
	 * @return Resulting texture.
	 */
	static shared<Texture2D> create(uint32_t width, uint32_t height, bool withAlpha = true);
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
