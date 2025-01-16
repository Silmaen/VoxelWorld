/**
 * @file TextureLibrary.h
 * @author Silmaen
 * @date 21/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "Texture.h"

namespace owl::renderer {

/**
 * @brief Class handling library of textures.
 */
class OWL_API TextureLibrary final {
public:
	/**
	 * @brief Constructor.
	 */
	TextureLibrary();

	/**
	 * @brief Destructor.
	 */
	~TextureLibrary();

	/**
	 * @brief Copy constructor.
	 */
	TextureLibrary(const TextureLibrary&) = default;

	/**
	 * @brief Move constructor.
	 */
	TextureLibrary(TextureLibrary&&) = default;

	/**
	 * @brief Copy affectation operator.
	 */
	auto operator=(const TextureLibrary&) -> TextureLibrary& = default;

	/**
	 * @brief Move affectation operator.
	 */
	auto operator=(TextureLibrary&&) -> TextureLibrary& = default;

	/**
	 * @brief Add the texture to the library.
	 * @param[in] iTexture Texture to add.
	 */
	void add(const shared<Texture>& iTexture);

	/**
	 * @brief Add the texture to the library and rename it.
	 * @param[in] iName Name of the texture.
	 * @param[in] iTexture The Texture to add.
	 */
	void addNRename(const std::string& iName, const shared<Texture>& iTexture);

	/**
	 * @brief Add the texture to the library from the standard path.
	 * @param[in] iName Name of the texture.
	 */
	void addFromStandardPath(const std::string& iName);

	/**
	 * @brief Load a texture from a file.
	 * @param[in] iFile The file to read for texture.
	 * @return The texture.
	 *
	 * The texture's name is determined by the file base name.
	 * @note 'tex.png' and 'tex.jpg' will be considered as same name.
	 */
	auto load(const std::filesystem::path& iFile) -> shared<Texture>;

	/**
	 * @brief Access to the texture of the given name.
	 * @param[in] iName Texture's name.
	 * @return Texture's pointer or nullptr if not exists.
	 */
	auto get(const std::string& iName) -> shared<Texture>;

	/**
	 * @brief Verify if a texture exists.
	 * @param[in] iName Texture's name.
	 * @return True if the texture exists.
	 */
	auto exists(const std::string& iName) const -> bool;

private:
	/// List of textures.
	std::unordered_map<std::string, shared<Texture>> m_textures;
};

}// namespace owl::renderer
