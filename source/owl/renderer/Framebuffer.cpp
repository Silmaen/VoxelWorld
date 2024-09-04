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
#include "vulkan/Framebuffer.h"

namespace owl::renderer {

auto Framebuffer::create(const FramebufferSpecification& iSpec) -> shared<Framebuffer> {
	switch (RenderCommand::getApi()) {
		case RenderAPI::Type::Null:
			return mkShared<null::Framebuffer>(iSpec);
		case RenderAPI::Type::OpenGL:
			return mkShared<opengl::Framebuffer>(iSpec);
		case RenderAPI::Type::Vulkan:
			return mkShared<vulkan::Framebuffer>(iSpec);
	}
	OWL_CORE_ERROR("Unknown API Type!")
	return nullptr;
}

Framebuffer::~Framebuffer() = default;

}// namespace owl::renderer
