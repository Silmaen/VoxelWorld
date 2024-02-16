/**
 * @file Framebuffer.cpp
 * @author Silmaen
 * @date 21/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Framebuffer.h"
#include "RenderAPI.h"
#include "Renderer.h"
#include "null/Framebuffer.h"
#include "opengl/Framebuffer.h"
#include "opengl_legacy/Framebuffer.h"
#include "vulkan/Framebuffer.h"

namespace owl::renderer {

shared<Framebuffer> Framebuffer::create(const FramebufferSpecification &spec) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::Null:
			return mk_shared<null::Framebuffer>(spec);
		case RenderAPI::Type::OpenGL:
			return mk_shared<opengl::Framebuffer>(spec);
		case RenderAPI::Type::OpenglLegacy:
			return mk_shared<opengl_legacy::Framebuffer>(spec);
		case RenderAPI::Type::Vulkan:
			return mk_shared<vulkan::Framebuffer>(spec);
	}
	OWL_CORE_ERROR("Unknown API Type!")
	return nullptr;
}

Framebuffer::~Framebuffer() = default;

}// namespace owl::renderer
