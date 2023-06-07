/**
 * @file Framebuffer.cpp
 * @author Silmaen
 * @date 21/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Framebuffer.h"
#include "gl_46/glad.h"

#include <utility>

namespace owl::renderer::opengl {

constexpr uint32_t maxFramebufferSize = 8192;

namespace utils {

static gl_46::GLenum textureTarget(bool multisampled) {
	return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void createTextures(bool multisampled, uint32_t *outID, uint32_t count) {
	gl_46::glCreateTextures(textureTarget(multisampled), static_cast<gl_46::GLsizei>(count), outID);
}

static void bindTexture(bool multisampled, uint32_t id) {
	gl_46::glBindTexture(textureTarget(multisampled), id);
}

static void attachColorTexture(uint32_t id, int samples, gl_46::GLenum internalFormat, gl_46::GLenum format, uint32_t width, uint32_t height, int index) {
	bool multisampled = samples > 1;
	if (multisampled) {
		gl_46::glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, static_cast<gl_46::GLsizei>(width), static_cast<gl_46::GLsizei>(height), GL_FALSE);
	} else {
		gl_46::glTexImage2D(GL_TEXTURE_2D, 0, static_cast<gl_46::GLint>(internalFormat), static_cast<gl_46::GLsizei>(width), static_cast<gl_46::GLsizei>(height), 0, format, GL_UNSIGNED_BYTE, nullptr);
		gl_46::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		gl_46::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gl_46::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		gl_46::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		gl_46::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	gl_46::glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<gl_46::GLenum>(GL_COLOR_ATTACHMENT0 + index), textureTarget(multisampled), id, 0);
}

static void attachDepthTexture(uint32_t id, int samples, gl_46::GLenum format, gl_46::GLenum attachmentType, uint32_t width, uint32_t height) {
	bool multisampled = samples > 1;
	if (multisampled) {
		gl_46::glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, static_cast<gl_46::GLsizei>(width), static_cast<gl_46::GLsizei>(height), GL_FALSE);
	} else {
		gl_46::glTexStorage2D(GL_TEXTURE_2D, 1, format, static_cast<gl_46::GLsizei>(width), static_cast<gl_46::GLsizei>(height));
		gl_46::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		gl_46::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gl_46::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		gl_46::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		gl_46::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	gl_46::glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multisampled), id, 0);
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

static gl_46::GLenum FBTextureFormatToGL(FramebufferTextureFormat format) {
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
	gl_46::glDeleteFramebuffers(1, &rendererID);
	gl_46::glDeleteTextures(static_cast<gl_46::GLsizei>(colorAttachments.size()), colorAttachments.data());
	gl_46::glDeleteTextures(1, &depthAttachment);
}

void Framebuffer::invalidate() {

	if (rendererID) {
		gl_46::glDeleteFramebuffers(1, &rendererID);
		gl_46::glDeleteTextures(static_cast<gl_46::GLsizei>(colorAttachments.size()), colorAttachments.data());
		gl_46::glDeleteTextures(1, &depthAttachment);
		colorAttachments.clear();
		depthAttachment = 0;
	}

	gl_46::glCreateFramebuffers(1, &rendererID);
	gl_46::glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

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
		gl_46::GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		gl_46::glDrawBuffers(static_cast<gl_46::GLsizei>(colorAttachments.size()), buffers);
	} else if (colorAttachments.empty()) {
		// Only depth-pass
		gl_46::glDrawBuffer(GL_NONE);
	}

	bool completeFramebuffer = gl_46::glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	OWL_CORE_ASSERT(completeFramebuffer, "Framebuffer is incomplete!")
	if (!completeFramebuffer) {
		OWL_CORE_WARN("Incomplete Framebuffer")
	}

	gl_46::glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Framebuffer::bind() {
	gl_46::glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
	gl_46::glViewport(0, 0, static_cast<gl_46::GLsizei>(specs.width), static_cast<gl_46::GLsizei>(specs.height));
}

void Framebuffer::unbind() {
	gl_46::glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	gl_46::glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	int pixelData;
	gl_46::glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
	return pixelData;
}

void Framebuffer::clearAttachment(uint32_t attachmentIndex, int value) {
	OWL_CORE_ASSERT(attachmentIndex < colorAttachments.size(), "clearAttachment bad attachment index")
	const auto &spec = colorAttachmentSpecifications[attachmentIndex];
	gl_46::glClearTexImage(colorAttachments[attachmentIndex], 0,
						   utils::FBTextureFormatToGL(spec.textureFormat), GL_INT, &value);
}

}// namespace owl::renderer::opengl
