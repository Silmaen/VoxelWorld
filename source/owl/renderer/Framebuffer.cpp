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

shared<Framebuffer> Framebuffer::create(const FramebufferSpecification &iSpec) {
	switch (RenderCommand::getApi()) {
		case RenderAPI::Type::Null:
			return mkShared<null::Framebuffer>(iSpec);
		case RenderAPI::Type::OpenGL:
			return mkShared<opengl::Framebuffer>(iSpec);
		case RenderAPI::Type::OpenglLegacy:
			return mkShared<opengl_legacy::Framebuffer>(iSpec);
		case RenderAPI::Type::Vulkan:
			return mkShared<vulkan::Framebuffer>(iSpec);
	}
	OWL_CORE_ERROR("Unknown API Type!")
	return nullptr;
}

Framebuffer::~Framebuffer() = default;

}// namespace owl::renderer
