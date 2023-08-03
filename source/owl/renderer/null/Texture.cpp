/**
 * @file Texture.cpp
 * @author Silmaen
 * @date 30/07/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Texture.h"

#include <stb_image.h>

#include <utility>

namespace owl::renderer::null {


Texture2D::Texture2D(uint32_t width_, uint32_t height_) : width{width_}, height{height_} {
																				 OWL_PROFILE_FUNCTION()}

														  Texture2D::Texture2D(std::filesystem::path path_) : path{std::move(path_)} {
	OWL_PROFILE_FUNCTION()

	int width_, height_, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc *data = nullptr;
	{
		OWL_PROFILE_SCOPE("stbi_load - Texture2D::Texture2D(const std::filesystem::path &)")
		data = stbi_load(path.string().c_str(), &width_, &height_, &channels, 0);
	}
	if (!data) {
		OWL_CORE_WARN("Failed to load image {}", path.string())
		return;
	}
	width = static_cast<uint32_t>(width_);
	height = static_cast<uint32_t>(height_);

	stbi_image_free(data);
}

Texture2D::~Texture2D() {
	OWL_PROFILE_FUNCTION()
}

void Texture2D::bind(uint32_t) const {
	OWL_PROFILE_FUNCTION()
}

void Texture2D::setData(void *, uint32_t) {
	OWL_PROFILE_FUNCTION()
}

}// namespace owl::renderer::null
