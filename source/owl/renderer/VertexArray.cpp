/**
 * @file VertexArray.cpp
 * @author Silmen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "VertexArray.h"

#include "Renderer.h"
#include "opengl/VertexArray.h"

namespace owl::renderer {

uniq<VertexArray> VertexArray::create() {
	switch (Renderer::getAPI()) {
		case APIType::None:
			OWL_CORE_ASSERT(false, "APIType::None is currently not supported!");
			return nullptr;
		case APIType::OpenGL:
			return mk_uniq<opengl::VertexArray>();
	}

	OWL_CORE_ASSERT(false, "Unknown Renderer API!");
	return nullptr;
}

}// namespace owl::renderer
