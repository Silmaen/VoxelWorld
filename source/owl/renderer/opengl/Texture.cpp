/**
 * @file Texture.cpp
 * @author Silmaen
 * @date 12/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Texture.h"
#include "core/external/opengl46.h"
#include <stb_image.h>
#include <utility>

namespace owl::renderer::opengl {

Texture2D::Texture2D(uint32_t width_, uint32_t height_, bool withAlpha) : width{width_}, height{height_}, hasAlpha{withAlpha} {
	OWL_PROFILE_FUNCTION()

	glCreateTextures(GL_TEXTURE_2D, 1, &textureId);
	glTextureStorage2D(textureId, 1, hasAlpha ? GL_RGBA8 : GL_RGB8, static_cast<GLsizei>(width), static_cast<GLsizei>(height));

	glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

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

	if (channels == 3)
		hasAlpha = false;

	if ((channels != 4) && (channels != 3)) {
		OWL_CORE_ERROR("Impossible to load {}, invalid number of channels {}: must be 3 or 4.")
		return;
	}
	width = static_cast<uint32_t>(width_);
	height = static_cast<uint32_t>(height_);

	glCreateTextures(GL_TEXTURE_2D, 1, &textureId);
	glTextureStorage2D(textureId, 1, hasAlpha ? GL_RGBA8 : GL_RGB8, static_cast<GLsizei>(width), static_cast<GLsizei>(height));

	glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(textureId, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

Texture2D::~Texture2D() {
	OWL_PROFILE_FUNCTION()

	glDeleteTextures(1, &textureId);
}

void Texture2D::bind(uint32_t slot) const {
	OWL_PROFILE_FUNCTION()

	glBindTextureUnit(slot, textureId);
}

void Texture2D::setData(void *data, [[maybe_unused]] uint32_t size) {
	OWL_PROFILE_FUNCTION()

	OWL_CORE_ASSERT(size == width * height * (hasAlpha ? 4 : 3), "Data size missmatch texture size!")
	glTextureSubImage2D(textureId, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
}

}// namespace owl::renderer::opengl
