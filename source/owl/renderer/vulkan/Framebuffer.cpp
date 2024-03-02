/**
 * @file Framebuffer.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Framebuffer.h"

namespace owl::renderer::vulkan {

Framebuffer::Framebuffer(FramebufferSpecification iSpec) : m_specs{std::move(iSpec)} {}

Framebuffer::~Framebuffer() = default;

void Framebuffer::invalidate() {}

void Framebuffer::bind() {}

void Framebuffer::unbind() {}

void Framebuffer::resize(const uint32_t iWidth, const uint32_t iHeight) {
	m_specs.width = iWidth;
	m_specs.height = iHeight;
}

int Framebuffer::readPixel(uint32_t, int, int) { return 0; }

void Framebuffer::clearAttachment(uint32_t, int) {}

}// namespace owl::renderer::vulkan
