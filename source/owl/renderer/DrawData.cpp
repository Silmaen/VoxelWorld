/**
 * @file DrawData.cpp
 * @author Silmaen
 * @date 04/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "DrawData.h"
#include "Renderer.h"
#include "null/DrawData.h"
#include "opengl/DrawData.h"
#include "opengl_legacy/DrawData.h"
#include <magic_enum.hpp>

namespace owl::renderer {

DrawData::~DrawData() = default;

shared<DrawData> DrawData::create() {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ERROR("Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::Null:
			return mk_shared<null::DrawData>();
		case RenderAPI::Type::OpenGL:
			return mk_shared<opengl::DrawData>();
		case RenderAPI::Type::OpenglLegacy:
			return mk_shared<opengl_legacy::DrawData>();
	}
	OWL_CORE_ERROR("Unknown API Type!")
	return nullptr;
}

}// namespace owl::renderer