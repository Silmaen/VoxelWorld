/**
 * @file RenderCommand.cpp
 * @author Silmaen
 * @date 09/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "RenderCommand.h"
#include <magic_enum.hpp>

namespace owl::renderer {

uniq<RenderAPI> RenderCommand::renderAPI = nullptr;

void RenderCommand::create(const RenderAPI::Type &type) {
	switch (type) {
		case RenderAPI::Type::OpenGL:
			renderAPI = RenderAPI::create(type);
			return;
		case RenderAPI::Type::None:
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ASSERT(false,"Render API {} is not yet supported", magic_enum::enum_name(type))
			return;
	}
	OWL_CORE_ASSERT(false, "Unknown Renderer API!")
	renderAPI = nullptr;
}

}// namespace owl::renderer
