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

shared<Texture2D> Texture2D::create(const std::filesystem::path &iFile) {
	switch (RenderCommand::getAPI()) {
		case RenderAPI::Type::Null: {
			if (auto texture = mkShared<null::Texture2D>(iFile); texture->isLoaded())// No data
				return texture;
			return nullptr;
		}
		case RenderAPI::Type::OpenGL: {
			if (auto texture = mkShared<opengl::Texture2D>(iFile); texture->isLoaded())// No data
				return texture;
			return nullptr;
		}
		case RenderAPI::Type::OpenglLegacy: {
			if (auto texture = mkShared<opengl_legacy::Texture2D>(iFile); texture->isLoaded())// No data
				return texture;
			return nullptr;
		}
		case RenderAPI::Type::Vulkan: {
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
	switch (RenderCommand::getAPI()) {
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
	switch (RenderCommand::getAPI()) {
		case RenderAPI::Type::Null:
			return mkShared<null::Texture2D>(iSize);
		case RenderAPI::Type::OpenGL:
			return mkShared<opengl::Texture2D>(iSize, iWithAlpha);
		case RenderAPI::Type::OpenglLegacy:
			return mkShared<opengl_legacy::Texture2D>(iSize, iWithAlpha);
		case RenderAPI::Type::Vulkan:
			return mkShared<vulkan::Texture2D>(iSize, iWithAlpha);
	}

	OWL_CORE_ERROR("Unknown RendererAPI!")
	return nullptr;
}

Texture::~Texture() = default;

}// namespace owl::renderer
