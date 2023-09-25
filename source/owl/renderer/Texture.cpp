/**
 * @file Texture.cpp
 * @author Silmaen
 * @date 12/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"
#include <magic_enum.hpp>

#include "Texture.h"

#include "Renderer.h"
#include "core/Application.h"
#include "null/Texture.h"
#include "opengl/Texture.h"
#include "opengl_legacy/Texture.h"

namespace owl::renderer {

shared<Texture2D> Texture2D::create(const std::filesystem::path &file) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ERROR("Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
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
	}

	OWL_CORE_ERROR("Unknown RendererAPI!")
	return nullptr;
}

shared<Texture2D> Texture2D::create(const std::string &textureName) {
	return create(core::Application::get().getAssetDirectory() / (textureName + ".png"));
}

shared<Texture2D> Texture2D::create(uint32_t width, uint32_t height) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ERROR("Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::Null:
			return mk_shared<null::Texture2D>(width, height);
		case RenderAPI::Type::OpenGL:
			return mk_shared<opengl::Texture2D>(width, height);
		case RenderAPI::Type::OpenglLegacy:
			return mk_shared<opengl_legacy::Texture2D>(width, height);
	}

	OWL_CORE_ERROR("Unknown RendererAPI!")
	return nullptr;
}

Texture::~Texture() = default;

}// namespace owl::renderer
