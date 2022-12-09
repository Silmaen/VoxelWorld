/**
 * @file Shader.cpp
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Renderer.h"
#include "Shader.h"
#include "opengl/Shader.h"
#include <magic_enum.hpp>

namespace owl::renderer {

shrd<Shader> Shader::create(const std::string &vertexSrc, const std::string &fragmentSrc) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::None:
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ASSERT("Render API {} is not yet supported", magic_enum::enum_name(type));
			return nullptr;
		case RenderAPI::Type::OpenGL:
			return mk_shrd<opengl::Shader>(vertexSrc, fragmentSrc);
	}
	OWL_CORE_ASSERT(false, "Unknown API Type!");
	return nullptr;
}

}// namespace owl::renderer
