/**
 * @file GraphContext.cpp
 * @author Silmen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "GraphContext.h"
#include "owlpch.h"
#include "renderer/opengl/GraphContext.h"

namespace owl::renderer {

uniq<GraphContext> GraphContext::Create(void *window) {
	//	switch (Renderer::GetAPI()) {
	//		case RendererAPI::API::None:
	//			OWL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
	//			return nullptr;
	//		case RendererAPI::API::OpenGL:
	return mk_uniq<opengl::GraphContext>(static_cast<GLFWwindow *>(window));
	//	}

	//	OWL_CORE_ASSERT(false, "Unknown RendererAPI!");
	//	return nullptr;
}

}// namespace owl::renderer