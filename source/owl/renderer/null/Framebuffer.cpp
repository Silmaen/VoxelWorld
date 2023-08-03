/**
 * @file Framebuffer.cpp
 * @author Silmaen
 * @date 30/07/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Framebuffer.h"

#include <utility>

namespace owl::renderer::null {

constexpr uint32_t maxFramebufferSize = 8192;

namespace utils {

static void createTextures(bool, uint32_t *, uint32_t) {}

static void bindTexture(bool, uint32_t) {}

static bool isDepthFormat(FramebufferTextureFormat format) {
	switch (format) {
		case FramebufferTextureFormat::DEPTH24STENCIL8:
			return true;
		case FramebufferTextureFormat::None:
		case FramebufferTextureFormat::RGBA8:
		case FramebufferTextureFormat::RED_INTEGER:
			return false;
	}
	return false;
}


}// namespace utils

Framebuffer::Framebuffer(FramebufferSpecification spec) : specs{std::move(spec)} {
	for (auto spec_: specs.attachments.attachments) {
		if (!utils::isDepthFormat(spec_.textureFormat))
			colorAttachmentSpecifications.emplace_back(spec_);
		else
			depthAttachmentSpecification = spec_;
	}
	invalidate();
}

Framebuffer::~Framebuffer() = default;

void Framebuffer::invalidate() {

	if (rendererID) {
		colorAttachments.clear();
		depthAttachment = 0;
	}


	bool multisample = specs.samples > 1;

	// Attachments
	if (!colorAttachmentSpecifications.empty()) {
		colorAttachments.resize(colorAttachmentSpecifications.size());
		utils::createTextures(multisample, colorAttachments.data(), static_cast<uint32_t>(colorAttachments.size()));

		for (size_t i = 0; i < colorAttachments.size(); i++) {
			utils::bindTexture(multisample, colorAttachments[i]);
			switch (colorAttachmentSpecifications[i].textureFormat) {
				case FramebufferTextureFormat::RGBA8:
				case FramebufferTextureFormat::RED_INTEGER:
				case FramebufferTextureFormat::None:
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					break;
			}
		}
	}

	if (depthAttachmentSpecification.textureFormat != FramebufferTextureFormat::None) {
		utils::createTextures(multisample, &depthAttachment, 1);
		utils::bindTexture(multisample, depthAttachment);
		switch (depthAttachmentSpecification.textureFormat) {
			case FramebufferTextureFormat::DEPTH24STENCIL8:
			case FramebufferTextureFormat::None:
			case FramebufferTextureFormat::RGBA8:
			case FramebufferTextureFormat::RED_INTEGER:
				break;
		}
	}

	if (colorAttachments.size() > 1) {
		OWL_CORE_ASSERT(colorAttachments.size() <= 4, "Bad color attachment size")
	} else if (colorAttachments.empty()) {
		// Only depth-pass
	}
}

void Framebuffer::bind() {
}

void Framebuffer::unbind() {
}

void Framebuffer::resize(uint32_t width, uint32_t height) {
	if (width == 0 || height == 0 || width > maxFramebufferSize || height > maxFramebufferSize) {
		OWL_CORE_WARN("Attempt to resize frame buffer to {} {}", width, height)
		return;
	}
	specs.width = width;
	specs.height = height;
	invalidate();
}

int Framebuffer::readPixel([[maybe_unused]] uint32_t attachmentIndex, int, int) {
	OWL_CORE_ASSERT(attachmentIndex < colorAttachments.size(), "ReadPixel bad attachment index")
	return 0;
}

void Framebuffer::clearAttachment([[maybe_unused]] uint32_t attachmentIndex, int) {
	OWL_CORE_ASSERT(attachmentIndex < colorAttachments.size(), "clearAttachment bad attachment index")
}

}// namespace owl::renderer::null
