/**
 * @file Texture.cpp
 * @author Silmen
 * @date 12/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"
#include <magic_enum.hpp>

#include "Texture.h"

#include "Renderer.h"
#include "opengl/Texture.h"

namespace owl::renderer {

shrd<Texture2D> Texture2D::create(const std::filesystem::path &file) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::OpenGL:
			return mk_shrd<opengl::Texture2D>(file);
		case RenderAPI::Type::Vulkan:
		case RenderAPI::Type::None:
			OWL_CORE_ASSERT(false,"Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
	}

	OWL_CORE_ASSERT(false, "Unknown RendererAPI!")
	return nullptr;
}
shrd<Texture2D> Texture2D::create(uint32_t width, uint32_t height) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::OpenGL:
			return mk_shrd<opengl::Texture2D>(width,height);
		case RenderAPI::Type::Vulkan:
		case RenderAPI::Type::None:
			OWL_CORE_ASSERT(false,"Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
	}

	OWL_CORE_ASSERT(false, "Unknown RendererAPI!")
	return nullptr;
}

}// namespace owl::renderer
