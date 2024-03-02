/**
 * @file Texture.h
 * @author Silmaen
 * @date 12/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"
#include "math/sizingTypes.h"

#include <filesystem>

namespace owl::renderer {
class TextureLibrary;
/**
 * @brief Class Texture.
 */
class OWL_API Texture {
public:
	Texture() = default;
	Texture(const Texture&) = default;
	Texture(Texture&&) = default;
	Texture& operator=(const Texture&) = default;
	Texture& operator=(Texture&&) = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~Texture();

	/**
	 * @brief Comparison operator.
	 * @param[in] iOther Other texture to compare.
	 * @return True if same.
	 */
	virtual bool operator==(const Texture& iOther) const = 0;

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
	 * @brief Access to texture's size.
	 * @return Texture's size.
	 */
	[[nodiscard]] virtual math::FrameSize getSize() const = 0;

	/**
	 * @brief Tells if the data effectively loaded.
	 * @return True if texture contains data.
	 */
	[[nodiscard]] virtual bool isLoaded() const = 0;

	/**
	 * @brief Get renderer id.
	 * @return The renderer ID.
	 */
	[[nodiscard]] virtual uint32_t getRendererId() const = 0;

	/**
	 * @brief Activate the texture in the GPU.
	 * @param[in] iSlot Slot into put the texture.
	 */
	virtual void bind(uint32_t iSlot) const = 0;

	/**
	 * @brief Get Path to texture file.
	 * @return Path to texture file.
	 */
	[[nodiscard]] virtual const std::filesystem::path& getPath() const = 0;

	/**
	 * @brief Define the texture data.
	 * @param[in] iData Raw data.
	 * @param[in] iSize Size of the data.
	 */
	virtual void setData(void* iData, uint32_t iSize) = 0;

	/**
	 * @brief Get access to the texture's name.
	 * @return The texture's name.
	 */
	[[nodiscard]] const std::string& getName() const { return m_name; }

private:
	/// The texture's name.
	std::string m_name;
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
	 * @param[in] iFile The path to the file to load.
	 * @return Resulting texture.
	 */
	static shared<Texture2D> create(const std::filesystem::path& iFile);

	/**
	 * @brief Create a new texture.
	 * @param[in] iTextureName Name of the files in the standard path.
	 * @return Pointer to the texture.
	 */
	static shared<Texture2D> create(const std::string& iTextureName);
	/**
	 * @brief Creates the texture with the given size.
	 * @param[in] iSize The texture's size.
	 * @param[in] iWithAlpha If the texture has alpha channel.
	 * @return Resulting texture.
	 */
	static shared<Texture2D> create(const math::FrameSize& iSize, bool iWithAlpha = true);
	/**
	 * @brief Creates the texture with the given size.
	 * @param[in] iWidth The texture's width.
	 * @param[in] iHeight The texture's height.
	 * @param[in] iWithAlpha If the texture has alpha channel.
	 * @return Resulting texture.
	 */
	static shared<Texture2D> create(uint32_t iWidth, uint32_t iHeight, bool iWithAlpha = true);
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif
} // namespace owl::renderer
