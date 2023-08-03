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
#include "null/VertexArray.h"
#include "opengl/VertexArray.h"
#include <magic_enum.hpp>

namespace owl::renderer {

shared<VertexArray> VertexArray::create() {
	auto type = Renderer::getAPI();
	switch (type) {
		//case RenderAPI::Type::None:
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ERROR("RenderAPI {} is currently not supported!", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::Null:
			return mk_shared<null::VertexArray>();
		case RenderAPI::Type::OpenGL:
			return mk_shared<opengl::VertexArray>();
	}

	OWL_CORE_ERROR("Unknown Renderer API!")
	return nullptr;
}
VertexArray::~VertexArray() = default;

}// namespace owl::renderer
