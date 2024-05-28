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

#include <utility>

namespace owl::renderer::opengl_legacy {

Texture2D::Texture2D(const math::FrameSize iSize, const bool iWithAlpha) : renderer::Texture2D{iSize, iWithAlpha} {
	OWL_PROFILE_FUNCTION()

	// functions safe for openGL 2.1
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(uint32_t iWidth, uint32_t iHeight, const bool iWithAlpha)
	: Texture2D({iWidth, iHeight}, iWithAlpha) {}

Texture2D::Texture2D(std::filesystem::path iPath) : renderer::Texture2D{std::move(iPath)} {
	OWL_PROFILE_FUNCTION()

	int width = 0;
	int height = 0;
	int channels = 0;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc *data = nullptr;
	{
		OWL_PROFILE_SCOPE("stbi_load - Texture2D::Texture2D(const std::filesystem::path &)")
		data = stbi_load(m_path.string().c_str(), &width, &height, &channels, 0);
	}
	if (!data) {
		OWL_CORE_WARN("Failed to load image {}", m_path.string())
		return;
	}

	if (channels == 3)
		m_hasAlpha = false;

	if ((channels != 4) && (channels != 3)) {
		OWL_CORE_ERROR("Impossible to load {}, invalid number of channels {}: must be 3 or 4.")
		return;
	}
	m_size = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, m_hasAlpha ? GL_RGBA : GL_RGB, static_cast<GLsizei>(m_size.width()),
				 static_cast<GLsizei>(m_size.height()), 0, m_hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

Texture2D::~Texture2D() {
	OWL_PROFILE_FUNCTION()

	glDeleteTextures(1, &m_textureId);
}

void Texture2D::bind(const uint32_t iSlot) const {
	OWL_PROFILE_FUNCTION()

	OWL_CORE_ASSERT(static_cast<int32_t>(iSlot) <
							std::max(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1, GL_MAX_TEXTURE_COORDS - 1),
					"Texture slot exceed capability")
	glActiveTexture(GL_TEXTURE0 + iSlot);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture2D::setData(void *iData, [[maybe_unused]] const uint32_t iSize) {
	OWL_PROFILE_FUNCTION()

	OWL_CORE_ASSERT(iSize == m_size.surface() * (m_hasAlpha ? 4 : 3), "Data size missmatch texture size!")

	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, m_hasAlpha ? GL_RGBA : GL_RGB, static_cast<GLsizei>(m_size.width()),
				 static_cast<GLsizei>(m_size.height()), 0, m_hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, iData);
	glBindTexture(GL_TEXTURE_2D, 0);
}

}// namespace owl::renderer::opengl_legacy
