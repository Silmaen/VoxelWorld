/**
 * @file Texture.cpp
 * @author Silmaen
 * @date 12/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Texture.h"

#include "Renderer.h"
#include "core/Application.h"
#include "null/Texture.h"
#include "opengl/Texture.h"
#include "opengl_legacy/Texture.h"
#include "vulkan/Texture.h"

namespace owl::renderer {


Texture::Texture(std::filesystem::path iPath) : m_path{std::move(iPath)} {}

Texture::Texture(const uint32_t iWidth, const uint32_t iHeight, const bool iWithAlpha)
	: m_size{iWidth, iHeight}, m_hasAlpha{iWithAlpha} {}

Texture::Texture(math::FrameSize iSize, const bool iWithAlpha) : m_size{iSize}, m_hasAlpha{iWithAlpha} {}


[[nodiscard]] std::string Texture::getSerializeString() const {
	if (!isLoaded()) {
		return "emp:";
	}
	if (!m_name.empty()) {
		return "nam:" + m_name;
	}
	if (!m_path.empty()) {
		return "pat:" + m_path.string();
	}
	return fmt::format("siz:{}:{}:{}", m_size.width(), m_size.height(), m_hasAlpha);
}

Texture2D::Texture2D(std::filesystem::path iPath) : Texture{std::move(iPath)} {}

Texture2D::Texture2D(const uint32_t iWidth, const uint32_t iHeight, const bool iWithAlpha)
	: Texture{iWidth, iHeight, iWithAlpha} {}

Texture2D::Texture2D(const math::FrameSize &iSize, const bool iWithAlpha) : Texture{iSize, iWithAlpha} {}

shared<Texture2D> Texture2D::create(const std::filesystem::path &iFile) {
	switch (RenderCommand::getApi()) {
		case RenderAPI::Type::Null:
			{
				if (auto texture = mkShared<null::Texture2D>(iFile); texture->isLoaded())// No data
					return texture;
				return nullptr;
			}
		case RenderAPI::Type::OpenGL:
			{
				if (auto texture = mkShared<opengl::Texture2D>(iFile); texture->isLoaded())// No data
					return texture;
				return nullptr;
			}
		case RenderAPI::Type::OpenglLegacy:
			{
				if (auto texture = mkShared<opengl_legacy::Texture2D>(iFile); texture->isLoaded())// No data
					return texture;
				return nullptr;
			}
		case RenderAPI::Type::Vulkan:
			{
				if (auto texture = mkShared<vulkan::Texture2D>(iFile); texture->isLoaded())// No data
					return texture;
				return nullptr;
			}
	}

	OWL_CORE_ERROR("Unknown RendererAPI!")
	return nullptr;
}

shared<Texture2D> Texture2D::create(const std::string &iTextureName) {
	return create(core::Application::get().getAssetDirectory() / (iTextureName + ".png"));
}

shared<Texture2D> Texture2D::create(uint32_t iWidth, uint32_t iHeight, bool iWithAlpha) {
	switch (RenderCommand::getApi()) {
		case RenderAPI::Type::Null:
			return mkShared<null::Texture2D>(iWidth, iHeight);
		case RenderAPI::Type::OpenGL:
			return mkShared<opengl::Texture2D>(iWidth, iHeight, iWithAlpha);
		case RenderAPI::Type::OpenglLegacy:
			return mkShared<opengl_legacy::Texture2D>(iWidth, iHeight, iWithAlpha);
		case RenderAPI::Type::Vulkan:
			return mkShared<vulkan::Texture2D>(iWidth, iHeight, iWithAlpha);
	}

	OWL_CORE_ERROR("Unknown RendererAPI!")
	return nullptr;
}

shared<Texture2D> Texture2D::create(const math::FrameSize &iSize, bool iWithAlpha) {
	shared<Texture2D> tex;
	switch (RenderCommand::getApi()) {
		case RenderAPI::Type::Null:
			tex = mkShared<null::Texture2D>(iSize);
			break;
		case RenderAPI::Type::OpenGL:
			tex = mkShared<opengl::Texture2D>(iSize, iWithAlpha);
			break;
		case RenderAPI::Type::OpenglLegacy:
			tex = mkShared<opengl_legacy::Texture2D>(iSize, iWithAlpha);
			break;
		case RenderAPI::Type::Vulkan:
			tex = mkShared<vulkan::Texture2D>(iSize, iWithAlpha);
			break;
	}
	if (tex) {
		tex->m_hasAlpha = iWithAlpha;
		return tex;
	}

	OWL_CORE_ERROR("Unknown RendererAPI!")
	return nullptr;
}

shared<Texture2D> Texture2D::createFromSerialized(const std::string &iTextureSerializedName) {
	if (iTextureSerializedName.size() < 4)
		return nullptr;
	const auto key = iTextureSerializedName.substr(0, 4);
	const auto val = iTextureSerializedName.substr(4);
	if (key == "emp:")
		return create(0, 0, false);
	if (key == "nam:")
		return create(val);
	if (key == "pat:")
		return create(std::filesystem::path(val));
	if (key == "siz:") {
		std::stringstream ss(val);
		std::string token;
		std::getline(ss, token, ':');
		const auto w = static_cast<uint32_t>(std::stoul(token));
		std::getline(ss, token, ':');
		const auto h = static_cast<uint32_t>(std::stoul(token));
		std::getline(ss, token, ':');
		const bool a = token != "false";
		return create(w, h, a);
	}
	return nullptr;
}

Texture::~Texture() = default;

}// namespace owl::renderer
