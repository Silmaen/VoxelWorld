/**
 * @file GraphContext.cpp
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "GraphContext.h"
#include "Renderer.h"
#include "null/GraphContext.h"
#include "opengl/GraphContext.h"
#include "opengl_legacy/GraphContext.h"
#include "vulkan/GraphContext.h"

#include <magic_enum.hpp>

namespace owl::renderer {

uniq<GraphContext> GraphContext::create(void *window) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::Null:
			return mk_uniq<null::GraphContext>(static_cast<GLFWwindow *>(window));
		case RenderAPI::Type::OpenGL:
			return mk_uniq<opengl::GraphContext>(static_cast<GLFWwindow *>(window));
		case RenderAPI::Type::OpenglLegacy:
			return mk_uniq<opengl_legacy::GraphContext>(static_cast<GLFWwindow *>(window));
		case RenderAPI::Type::Vulkan:
			return mk_uniq<vulkan::GraphContext>(static_cast<GLFWwindow *>(window));
	}

	OWL_CORE_ERROR("Unknown RendererAPI!")
	return nullptr;
}
GraphContext::~GraphContext() = default;

}// namespace owl::renderer
