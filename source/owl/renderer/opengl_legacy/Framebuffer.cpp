/**
 * @file Framebuffer.cpp
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Framebuffer.h"

namespace owl::renderer::opengl_legacy {

Framebuffer::Framebuffer(FramebufferSpecification spec) : specs{std::move(spec)} {
	OWL_CORE_ERROR("Legacy OpenGL has no frame buffer support.")
	rendererID = 0;
}

Framebuffer::~Framebuffer() = default;

void Framebuffer::invalidate() {
}

void Framebuffer::bind() {
}

void Framebuffer::unbind() {
}

void Framebuffer::resize(uint32_t width, uint32_t height) {
	specs.width = width;
	specs.height = height;
}

int Framebuffer::readPixel(uint32_t, int, int) {
	return 0;
}

void Framebuffer::clearAttachment(uint32_t, int) {
}

}// namespace owl::renderer::opengl_legacy
