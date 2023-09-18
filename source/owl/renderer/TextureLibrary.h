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
 * @brief Class TextureLibrary
 */
class OWL_API TextureLibrary {
public:
	/**
	 * @brief Constructor.
	 */
	TextureLibrary();

	/**
	 * @brief Destructor.
	 */
	virtual ~TextureLibrary();

	/**
	 * @brief Copy constructor.
	 */
	TextureLibrary(const TextureLibrary &) = default;

	/**
	 * @brief Move constructor.
	 */
	TextureLibrary(TextureLibrary &&) = default;

	/**
	 * @brief Copy affectation operator.
	 */
	TextureLibrary &operator=(const TextureLibrary &) = default;

	/**
	 * @brief Move affectation operator.
	 */
	TextureLibrary &operator=(TextureLibrary &&) = default;

	/**
	 * @brief Add the texture to the library.
	 * @param texture Texture to add.
	 */
	void add(const shared<Texture> &texture);

	/**
	 * @brief Add the texture to the library and rename it.
	 * @param name Name of the texture.
	 * @param texture The Texture to add.
	 */
	void addNRename(const std::string &name, const shared<Texture> &texture);

	/**
	 * @brief Add the texture to the library from the standard path.
	 * @param name Name of the texture.
	 */
	void addFromStandardPath(const std::string &name);

	/**
	 * @brief Load a texture from a file.
	 * @param file The file to read for texture.
	 * @return The texture.
	 *
	 * The texture's name is determined by the file base name.
	 * @note 'tex.png' and 'tex.jpg' will be considered as same name.
	 */
	shared<Texture> load(const std::filesystem::path &file);

	/**
	 * @brief Access to the texture of the given name.
	 * @param name Texture's name.
	 * @return Texture's pointer or nullptr if not exists.
	 */
	shared<Texture> get(const std::string &name);

	/**
	 * @brief Verify if a texture exists.
	 * @param name Texture's name.
	 * @return True if the texture exists.
	 */
	bool exists(const std::string &name) const;

private:
	/// List of textures.
	std::unordered_map<std::string, shared<Texture>> textures;
};

}// namespace owl::renderer
