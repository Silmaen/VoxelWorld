/**
 * @file RenderAPI.cpp
 * @author Silmaen
 * @date 09/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "RenderAPI.h"
#include "null/RenderAPI.h"
#include "opengl/RenderAPI.h"
#include "opengl_legacy/RenderAPI.h"
#include "vulkan/RenderAPI.h"

namespace owl::renderer {


uniq<RenderAPI> RenderAPI::create(const RenderAPI::Type &iType) {
	switch (iType) {
		case Type::Null:
			return mkUniq<null::RenderAPI>();
		case Type::OpenGL:
			return mkUniq<opengl::RenderAPI>();
		case Type::OpenglLegacy:
			return mkUniq<opengl_legacy::RenderAPI>();
		case Type::Vulkan:
			return mkUniq<vulkan::RenderAPI>();
	}

	OWL_CORE_ERROR("Unknown RendererAPI!")
	return nullptr;
}

RenderAPI::~RenderAPI() = default;

}// namespace owl::renderer
