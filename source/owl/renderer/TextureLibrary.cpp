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
	for (auto &texture: textures)
		texture.second.reset();
	textures.clear();
}

void TextureLibrary::add(const shared<Texture> &texture) {
	if (exists(texture->getName())) {
		OWL_CORE_WARN("Texture {} Already in the library", texture->getName())
		return;
	}
	OWL_CORE_TRACE("Texture {} Added from name.", texture->getName())
	textures[texture->getName()] = texture;
}

void TextureLibrary::addNRename(const std::string &name, const shared<Texture> &texture) {
	if (exists(name)) {
		OWL_CORE_WARN("Texture {} Already in the library", name)
		return;
	}
	texture->name = name;
	OWL_CORE_TRACE("Texture {} Added from filename {}.", name, texture->getName())
	textures[name] = texture;
}

void TextureLibrary::addFromStandardPath(const std::string &name) {
	if (exists(name)) {
		OWL_CORE_WARN("Texture {} Already in the library", name)
		return;
	}
	OWL_CORE_TRACE("Texture {} Added.", name)
	textures[name] = Texture2D::create(name);
}

shared<Texture> TextureLibrary::load(const std::filesystem::path &file) {
	shared<Texture> texture = Texture2D::create(file);
	if (texture == nullptr) {
		OWL_CORE_WARN("Could not load texture file {}", file.string())
		return nullptr;
	}
	OWL_CORE_TRACE("Texture {} Added from file.", texture->getName())
	textures[texture->getName()] = texture;
	return texture;
}

shared<Texture> TextureLibrary::get(const std::string &name) {
	if (!exists(name)) {
		OWL_CORE_ERROR("Texture {} not found in library", name)
		return nullptr;
	}
	return textures[name];
}

bool TextureLibrary::exists(const std::string &name) const {
	return textures.find(name) != textures.end();
}

}// namespace owl::renderer
