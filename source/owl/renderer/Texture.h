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
	Texture(const Texture&) = default;
	Texture(Texture&&) = default;
	auto operator=(const Texture&) -> Texture& = default;
	auto operator=(Texture&&) -> Texture& = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~Texture();
	/**
	 * @brief Default constructor.
	 * @param[in] iPath path to the texture image file.
	 */
	explicit Texture(std::filesystem::path iPath);
	/**
	 * @brief Constructor by size.
	 * @param[in] iWidth Texture's width.
	 * @param[in] iHeight Texture's height.
	 * @param[in] iWithAlpha Texture has alpha channel.
	 */
	Texture(uint32_t iWidth, uint32_t iHeight, bool iWithAlpha);
	/**
	 * @brief Constructor by size.
	 * @param[in] iSize Texture's width.
	 * @param[in] iWithAlpha Texture has alpha channel.
	 */
	explicit Texture(const math::vec2ui& iSize, bool iWithAlpha = true);

	/**
	 * @brief Comparison operator.
	 * @param[in] iOther Other texture to compare.
	 * @return True if same.
	 */
	virtual auto operator==(const Texture& iOther) const -> bool = 0;

	/**
	 * @brief Access to texture's size.
	 * @return Texture's size.
	 */
	[[nodiscard]] auto getSize() const -> math::vec2ui { return m_size; }

	/**
	 * @brief Tells if the data effectively loaded.
	 * @return True if texture contains data.
	 */
	[[nodiscard]] auto isLoaded() const -> bool { return m_size.surface() > 0; }

	/**
	 * @brief Get renderer id.
	 * @return The renderer ID.
	 */
	[[nodiscard]] virtual auto getRendererId() const -> uint64_t = 0;

	/**
	 * @brief Activate the texture in the GPU.
	 * @param[in] iSlot Slot into put the texture.
	 */
	virtual void bind(uint32_t iSlot) const = 0;

	/**
	 * @brief Get Path to texture file.
	 * @return Path to texture file.
	 */
	[[nodiscard]] auto getPath() const -> const std::filesystem::path& { return m_path; }

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
	[[nodiscard]] auto getName() const -> const std::string& { return m_name; }

	/**
	 * @brief Get a string that can be serialized.
	 * @return A serialized String.
	 */
	[[nodiscard]] auto getSerializeString() const -> std::string;

protected:
	/// Texture's size.
	math::vec2ui m_size = {0, 0};
	/// Path to the texture file.
	std::filesystem::path m_path;
	/// If texture has alpha.
	bool m_hasAlpha{false};

private:
	/// The texture's name.
	std::string m_name;
	/// Library is a friend to be able to modify name.
	friend class TextureLibrary;
};

/**
 * @brief Class texture 2D
 */
class OWL_API Texture2D : public Texture {
public:
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
	 * @brief Destructor.
	 */
	~Texture2D() override;
	/**
	 * @brief Constructor by size.
	 * @param[in] iSize Texture's width.
	 * @param[in] iWithAlpha Texture has alpha channel.
	 */
	explicit Texture2D(const math::vec2ui& iSize, bool iWithAlpha = true);
	/**
	 * @brief Creates the texture with the given filename.
	 * @param[in] iFile The path to the file to load.
	 * @return Resulting texture.
	 */
	static auto create(const std::filesystem::path& iFile) -> shared<Texture2D>;

	/**
	 * @brief Create a new texture.
	 * @param[in] iTextureName Name of the files in the standard path.
	 * @return Pointer to the texture.
	 */
	static auto create(const std::string& iTextureName) -> shared<Texture2D>;

	/**
	 * @brief Create a new texture.
	 * @param[in] iTextureSerializedName Name of the files in the standard path.
	 * @return Pointer to the texture.
	 */
	static auto createFromSerialized(const std::string& iTextureSerializedName) -> shared<Texture2D>;

	/**
	 * @brief Creates the texture with the given size.
	 * @param[in] iSize The texture's size.
	 * @param[in] iWithAlpha If the texture has alpha channel.
	 * @return Resulting texture.
	 */
	static auto create(const math::vec2ui& iSize, bool iWithAlpha = true) -> shared<Texture2D>;
	/**
	 * @brief Creates the texture with the given size.
	 * @param[in] iWidth The texture's width.
	 * @param[in] iHeight The texture's height.
	 * @param[in] iWithAlpha If the texture has alpha channel.
	 * @return Resulting texture.
	 */
	static auto create(uint32_t iWidth, uint32_t iHeight, bool iWithAlpha = true) -> shared<Texture2D>;
};

}// namespace owl::renderer
