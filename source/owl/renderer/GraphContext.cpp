/**
 * @file GraphContext.cpp
 * @author Silmen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "GraphContext.h"
#include "Renderer.h"
#include "renderer/opengl/GraphContext.h"

#include <magic_enum.hpp>

namespace owl::renderer {

uniq<GraphContext> GraphContext::create(void *window) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::None:
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ASSERT(false, "Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::OpenGL:
			return mk_uniq<opengl::GraphContext>(static_cast<GLFWwindow *>(window));
	}

	OWL_CORE_ASSERT(false, "Unknown RendererAPI!")
	return nullptr;
}

}// namespace owl::renderer
