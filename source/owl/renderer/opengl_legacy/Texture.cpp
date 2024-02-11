/**
 * @file Texture.cpp
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Texture.h"

#include "core/external/opengl21.h"
#include <stb_image.h>

namespace owl::renderer::opengl_legacy {

Texture2D::Texture2D(const math::FrameSize &size_, bool withAlpha) : size{size_}, hasAlpha{withAlpha} {
	OWL_PROFILE_FUNCTION()

	// functions safe for openGL 2.1
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(uint32_t width_, uint32_t height_, bool withAlpha) : Texture2D({width_, height_}, withAlpha) {
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
	size = {static_cast<uint32_t>(width_), static_cast<uint32_t>(height_)};

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA : GL_RGB,
				 static_cast<GLsizei>(size.width()),
				 static_cast<GLsizei>(size.height()),
				 0,
				 hasAlpha ? GL_RGBA : GL_RGB,
				 GL_UNSIGNED_BYTE,
				 data);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

Texture2D::~Texture2D() {
	OWL_PROFILE_FUNCTION()

	glDeleteTextures(1, &textureId);
}

void Texture2D::bind(uint32_t slot) const {
	OWL_PROFILE_FUNCTION()

	OWL_CORE_ASSERT(static_cast<int32_t>(slot) < std::max(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1, GL_MAX_TEXTURE_COORDS - 1), "Texture slot exceed capability")
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture2D::setData(void *data, [[maybe_unused]] uint32_t size_) {
	OWL_PROFILE_FUNCTION()

	OWL_CORE_ASSERT(size_ == size.surface() * (hasAlpha ? 4 : 3), "Data size missmatch texture size!")

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA : GL_RGB,
				 static_cast<GLsizei>(size.width()),
				 static_cast<GLsizei>(size.height()),
				 0,
				 hasAlpha ? GL_RGBA : GL_RGB,
				 GL_UNSIGNED_BYTE,
				 data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

}// namespace owl::renderer::opengl_legacy
