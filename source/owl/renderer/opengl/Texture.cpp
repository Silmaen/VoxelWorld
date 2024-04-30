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

namespace owl::renderer::opengl {

Texture2D::Texture2D(math::FrameSize iSize, const bool iWithAlpha) : renderer::Texture2D{iSize, iWithAlpha} {
	OWL_PROFILE_FUNCTION()

	glCreateTextures(GL_TEXTURE_2D, 1, &m_textureId);
	glTextureStorage2D(m_textureId, 1, m_hasAlpha ? GL_RGBA8 : GL_RGB8, static_cast<GLsizei>(m_size.width()),
					   static_cast<GLsizei>(m_size.height()));

	glTextureParameteri(m_textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture2D::Texture2D(uint32_t iWidth, uint32_t iHeight, const bool iWithAlpha)
	: Texture2D({iWidth, iHeight}, iWithAlpha) {}

Texture2D::Texture2D(std::filesystem::path iPath) : renderer::Texture2D{std::move(iPath)} {
	OWL_PROFILE_FUNCTION()

	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc *data;
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

	glCreateTextures(GL_TEXTURE_2D, 1, &m_textureId);
	glTextureStorage2D(m_textureId, 1, m_hasAlpha ? GL_RGBA8 : GL_RGB8, static_cast<GLsizei>(m_size.width()),
					   static_cast<GLsizei>(m_size.height()));

	glTextureParameteri(m_textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(m_textureId, 0, 0, 0, static_cast<GLsizei>(m_size.width()),
						static_cast<GLsizei>(m_size.height()), m_hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

Texture2D::~Texture2D() {
	OWL_PROFILE_FUNCTION()

	glDeleteTextures(1, &m_textureId);
}

void Texture2D::bind(const uint32_t iSlot) const {
	OWL_PROFILE_FUNCTION()

	glBindTextureUnit(iSlot, m_textureId);
}

void Texture2D::setData(void *iData, [[maybe_unused]] const uint32_t iSize) {
	OWL_PROFILE_FUNCTION()

	OWL_CORE_ASSERT(iSize == m_size.surface() * (m_hasAlpha ? 4 : 3), "Data size missmatch texture size!")
	glTextureSubImage2D(m_textureId, 0, 0, 0, static_cast<GLsizei>(m_size.width()),
						static_cast<GLsizei>(m_size.height()), m_hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, iData);
}

}// namespace owl::renderer::opengl
