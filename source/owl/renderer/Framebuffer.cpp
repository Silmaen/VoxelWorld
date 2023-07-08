/**
 * @file Framebuffer.cpp
 * @author Silmaen
 * @date 21/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include <magic_enum.hpp>

#include "Framebuffer.h"
#include "RenderAPI.h"
#include "Renderer.h"
#include "opengl/Framebuffer.h"

namespace owl::renderer {

shared<Framebuffer> Framebuffer::create(const FramebufferSpecification &spec) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::None:
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ASSERT(false, "Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::OpenGL:
			return mk_shared<opengl::Framebuffer>(spec);
	}
	OWL_CORE_ASSERT(false, "Unknown API Type!")
	return nullptr;
}

}// namespace owl::renderer
