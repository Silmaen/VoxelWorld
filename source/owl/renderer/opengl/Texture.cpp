/**
 * @file Texture.cpp
 * @author Silmaen
 * @date 12/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace owl::renderer::opengl {


Texture2D::Texture2D(uint32_t width_, uint32_t height_):width{width_},height{height_} {
	OWL_PROFILE_FUNCTION()

	internalFormat = GL_RGBA8;
	dataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
	glTextureStorage2D(rendererID, 1, internalFormat, width, height);

	glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture2D::Texture2D(const std::filesystem::path &path_) : path{path_} {
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

	if (channels == 4) {
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	} else if (channels == 3) {
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}

	OWL_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!")
	glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
	glTextureStorage2D(rendererID, 1, internalFormat, width, height);

	glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(rendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

Texture2D::~Texture2D() {
	OWL_PROFILE_FUNCTION()

	glDeleteTextures(1, &rendererID);
}

void Texture2D::bind(uint32_t slot) const {
	OWL_PROFILE_FUNCTION()

	glBindTextureUnit(slot, rendererID);
}

void Texture2D::setData(void *data, [[maybe_unused]] uint32_t size) {
	OWL_PROFILE_FUNCTION()

	[[maybe_unused]] uint32_t bpp = dataFormat == GL_RGBA ? 4 : 3;
	OWL_CORE_ASSERT(size == width * height * bpp, "Data must be entire texture!")
	glTextureSubImage2D(rendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
}

}// namespace owl::renderer::opengl
