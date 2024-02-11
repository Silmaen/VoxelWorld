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

shared<Texture2D> Texture2D::create(const std::filesystem::path &file) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::Null: {
			auto texture = mk_shared<null::Texture2D>(file);
			if (texture->isLoaded())// No data
				return texture;
			return nullptr;
		}
		case RenderAPI::Type::OpenGL: {
			auto texture = mk_shared<opengl::Texture2D>(file);
			if (texture->isLoaded())// No data
				return texture;
			return nullptr;
		}
		case RenderAPI::Type::OpenglLegacy: {
			auto texture = mk_shared<opengl_legacy::Texture2D>(file);
			if (texture->isLoaded())// No data
				return texture;
			return nullptr;
		}
		case RenderAPI::Type::Vulkan: {
			auto texture = mk_shared<vulkan::Texture2D>(file);
			if (texture->isLoaded())// No data
				return texture;
			return nullptr;
		}
	}

	OWL_CORE_ERROR("Unknown RendererAPI!")
	return nullptr;
}

shared<Texture2D> Texture2D::create(const std::string &textureName) {
	return create(core::Application::get().getAssetDirectory() / (textureName + ".png"));
}

shared<Texture2D> Texture2D::create(uint32_t width, uint32_t height, bool withAlpha) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::Null:
			return mk_shared<null::Texture2D>(width, height);
		case RenderAPI::Type::OpenGL:
			return mk_shared<opengl::Texture2D>(width, height, withAlpha);
		case RenderAPI::Type::OpenglLegacy:
			return mk_shared<opengl_legacy::Texture2D>(width, height, withAlpha);
		case RenderAPI::Type::Vulkan:
			return mk_shared<vulkan::Texture2D>(width, height, withAlpha);
	}

	OWL_CORE_ERROR("Unknown RendererAPI!")
	return nullptr;
}
shared<Texture2D> Texture2D::create(const math::FrameSize &size, bool withAlpha) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::Null:
			return mk_shared<null::Texture2D>(size);
		case RenderAPI::Type::OpenGL:
			return mk_shared<opengl::Texture2D>(size, withAlpha);
		case RenderAPI::Type::OpenglLegacy:
			return mk_shared<opengl_legacy::Texture2D>(size, withAlpha);
		case RenderAPI::Type::Vulkan:
			return mk_shared<vulkan::Texture2D>(size, withAlpha);
	}

	OWL_CORE_ERROR("Unknown RendererAPI!")
	return nullptr;
}

Texture::~Texture() = default;

}// namespace owl::renderer
