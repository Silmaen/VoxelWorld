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

namespace owl::renderer {

RenderAPI::Type RenderAPI::type = RenderAPI::Type::Null;
RenderAPI::State RenderAPI::state = RenderAPI::State::Created;

uniq<RenderAPI> RenderAPI::create(const RenderAPI::Type &creationType) {
	state = RenderAPI::State::Created;
	type = creationType;
	switch (type) {
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ERROR("Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::Null:
			return mk_uniq<null::RenderAPI>();
		case RenderAPI::Type::OpenGL:
			return mk_uniq<opengl::RenderAPI>();
		case RenderAPI::Type::OpenglLegacy:
			return mk_uniq<opengl_legacy::RenderAPI>();
	}

	OWL_CORE_ERROR("Unknown RendererAPI!")
	return nullptr;
}
RenderAPI::~RenderAPI() = default;

}// namespace owl::renderer
