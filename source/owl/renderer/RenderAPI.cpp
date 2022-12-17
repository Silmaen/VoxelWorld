/**
 * @file RenderAPI.cpp
 * @author Silmaen
 * @date 09/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "RenderAPI.h"
#include "opengl/RenderAPI.h"

#include <magic_enum.hpp>

namespace owl::renderer {

RenderAPI::Type RenderAPI::type = RenderAPI::Type::None;

uniq<RenderAPI> RenderAPI::create(const RenderAPI::Type& type_){
	switch (type_) {
		case RenderAPI::Type::None:
		case RenderAPI::Type::Vulkan:
			type =type_;
			OWL_CORE_ASSERT(false,"Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::OpenGL:
			type =type_;
			return mk_uniq<opengl::RenderAPI>();
	}

	OWL_CORE_ASSERT(false, "Unknown RendererAPI!")
	return nullptr;
}

}// namespace owl::renderer
