/**
 * @file Framebuffer.cpp
 * @author Silmaen
 * @date 21/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Framebuffer.h"
#include <glad/glad.h>

#include <utility>

namespace owl::renderer::opengl {

constexpr uint32_t maxFramebufferSize = 8192;

namespace utils {

static GLenum textureTarget(bool multisampled) {
	return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void createTextures(bool multisampled, uint32_t *outID, uint32_t count) {
	glCreateTextures(textureTarget(multisampled), count, outID);
}

static void bindTexture(bool multisampled, uint32_t id) {
	glBindTexture(textureTarget(multisampled), id);
}

static void attachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
	bool multisampled = samples > 1;
	if (multisampled) {
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget(multisampled), id, 0);
}

static void attachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
	bool multisampled = samples > 1;
	if (multisampled) {
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
	} else {
		glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multisampled), id, 0);
}

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

static GLenum FBTextureFormatToGL(FramebufferTextureFormat format) {
	switch (format) {
		case FramebufferTextureFormat::RGBA8:
			return GL_RGBA8;
		case FramebufferTextureFormat::RED_INTEGER:
			return GL_RED_INTEGER;
		case FramebufferTextureFormat::None:
		case FramebufferTextureFormat::DEPTH24STENCIL8:
			break;
	}
	OWL_CORE_ASSERT(false, "Bad Texture format")
	return 0;
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

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &rendererID);
	glDeleteTextures(colorAttachments.size(), colorAttachments.data());
	glDeleteTextures(1, &depthAttachment);
}

void Framebuffer::invalidate() {

	if (rendererID) {
		glDeleteFramebuffers(1, &rendererID);
		glDeleteTextures(colorAttachments.size(), colorAttachments.data());
		glDeleteTextures(1, &depthAttachment);
		colorAttachments.clear();
		depthAttachment = 0;
	}

	glCreateFramebuffers(1, &rendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

	bool multisample = specs.samples > 1;

	// Attachments
	if (!colorAttachmentSpecifications.empty()) {
		colorAttachments.resize(colorAttachmentSpecifications.size());
		utils::createTextures(multisample, colorAttachments.data(), static_cast<uint32_t>(colorAttachments.size()));

		for (size_t i = 0; i < colorAttachments.size(); i++) {
			utils::bindTexture(multisample, colorAttachments[i]);
			switch (colorAttachmentSpecifications[i].textureFormat) {
				case FramebufferTextureFormat::RGBA8:
					utils::attachColorTexture(colorAttachments[i], static_cast<int>(specs.samples), GL_RGBA8, GL_RGBA, specs.width, specs.height, static_cast<int>(i));
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					utils::attachColorTexture(colorAttachments[i], static_cast<int>(specs.samples), GL_R32I, GL_RED_INTEGER, specs.width, specs.height, static_cast<int>(i));
					break;
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
				utils::attachDepthTexture(depthAttachment, static_cast<int>(specs.samples), GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, specs.width, specs.height);
				break;
			case FramebufferTextureFormat::None:
			case FramebufferTextureFormat::RGBA8:
			case FramebufferTextureFormat::RED_INTEGER:
				break;
		}
	}

	if (colorAttachments.size() > 1) {
		OWL_CORE_ASSERT(colorAttachments.size() <= 4, "Bad color attachment size")
		GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glDrawBuffers(colorAttachments.size(), buffers);
	} else if (colorAttachments.empty()) {
		// Only depth-pass
		glDrawBuffer(GL_NONE);
	}

	bool completeFramebuffer = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	OWL_CORE_ASSERT(completeFramebuffer, "Framebuffer is incomplete!")
	if(!completeFramebuffer) {
		OWL_CORE_WARN("Incomplete Framebuffer")
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Framebuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
	glViewport(0, 0, specs.width, specs.height);
}

void Framebuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

int Framebuffer::readPixel(uint32_t attachmentIndex, int x, int y) {
	OWL_CORE_ASSERT(attachmentIndex < colorAttachments.size(), "ReadPixel bad attachment index")

	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	int pixelData;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
	return pixelData;
}

void Framebuffer::clearAttachment(uint32_t attachmentIndex, int value) {
	OWL_CORE_ASSERT(attachmentIndex < colorAttachments.size(), "clearAttachment bad attachment index")
	const auto &spec = colorAttachmentSpecifications[attachmentIndex];
	glClearTexImage(colorAttachments[attachmentIndex], 0,
					utils::FBTextureFormatToGL(spec.textureFormat), GL_INT, &value);
}

}// namespace owl::renderer::opengl
