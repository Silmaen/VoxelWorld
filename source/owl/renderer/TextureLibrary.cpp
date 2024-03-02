/**
 * @file TextureLibrary.cpp
 * @author Silmaen
 * @date 21/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "TextureLibrary.h"

namespace owl::renderer {

TextureLibrary::TextureLibrary() = default;

TextureLibrary::~TextureLibrary() {
	for (auto &texture: m_textures)
		texture.second.reset();
	m_textures.clear();
}

void TextureLibrary::add(const shared<Texture> &iTexture) {
	if (exists(iTexture->getName())) {
		OWL_CORE_WARN("Texture {} Already in the library", iTexture->getName())
		return;
	}
	OWL_CORE_TRACE("Texture {} Added from name.", iTexture->getName())
	m_textures[iTexture->getName()] = iTexture;
}

void TextureLibrary::addNRename(const std::string &iName, const shared<Texture> &iTexture) {
	if (exists(iName)) {
		OWL_CORE_WARN("Texture {} Already in the library", iName)
		return;
	}
	iTexture->m_name = iName;
	OWL_CORE_TRACE("Texture {} Added from filename {}.", iName, iTexture->getName())
	m_textures[iName] = iTexture;
}

void TextureLibrary::addFromStandardPath(const std::string &iName) {
	if (exists(iName)) {
		OWL_CORE_WARN("Texture {} Already in the library", iName)
		return;
	}
	OWL_CORE_TRACE("Texture {} Added.", iName)
	m_textures[iName] = Texture2D::create(iName);
}

shared<Texture> TextureLibrary::load(const std::filesystem::path &iFile) {
	shared<Texture> texture = Texture2D::create(iFile);
	if (texture == nullptr) {
		OWL_CORE_WARN("Could not load texture file {}", iFile.string())
		return nullptr;
	}
	OWL_CORE_TRACE("Texture {} Added from file.", texture->getName())
	m_textures[texture->getName()] = texture;
	return texture;
}

shared<Texture> TextureLibrary::get(const std::string &iName) {
	if (!exists(iName)) {
		OWL_CORE_ERROR("Texture {} not found in library", iName)
		return nullptr;
	}
	return m_textures[iName];
}

bool TextureLibrary::exists(const std::string &iName) const { return m_textures.contains(iName); }

}// namespace owl::renderer
