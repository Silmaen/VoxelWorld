/**
 * @file Framebuffer.cpp
 * @author Silmaen
 * @date 30/06/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Framebuffer.h"
#include "gl_21/glad.h"

#include <utility>

namespace owl::renderer::opengl_legacy {

constexpr uint32_t maxFramebufferSize = 8192;

namespace utils {

static gl_21::GLenum textureTarget(bool multisampled) {
	// todo: OpenGL 3
	return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void bindTexture(bool multisampled, uint32_t id) {
	gl_21::glBindTexture(textureTarget(multisampled), id);
}

static void createTextures(bool multisampled, uint32_t *outID, uint32_t count) {
	gl_21::glGenTextures(static_cast<gl_21::GLsizei>(count), outID);
	bindTexture(multisampled, *outID);
}

static void attachColorTexture(uint32_t id, int samples, gl_21::GLenum internalFormat, gl_21::GLenum format, uint32_t width, uint32_t height, int index) {
	bool multisampled = samples > 1;
	if (multisampled) {
		// todo: OpenGL 3
		gl_21::glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, static_cast<gl_21::GLsizei>(width), static_cast<gl_21::GLsizei>(height), GL_FALSE);
	} else {
		gl_21::glTexImage2D(GL_TEXTURE_2D, 0, static_cast<gl_21::GLint>(internalFormat), static_cast<gl_21::GLsizei>(width), static_cast<gl_21::GLsizei>(height), 0, format, GL_UNSIGNED_BYTE, nullptr);
		gl_21::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		gl_21::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gl_21::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		gl_21::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		gl_21::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	// warning OpenGL 3
	gl_21::glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<gl_21::GLenum>(GL_COLOR_ATTACHMENT0 + index), textureTarget(multisampled), id, 0);
}

static void attachDepthTexture(uint32_t id, int samples, gl_21::GLenum format, gl_21::GLenum attachmentType, uint32_t width, uint32_t height) {
	bool multisampled = samples > 1;
	if (multisampled) {
		// todo: OpenGL 3
		gl_21::glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, static_cast<gl_21::GLsizei>(width), static_cast<gl_21::GLsizei>(height), GL_FALSE);
	} else {
		// todo: OpenGL 4
		gl_21::glTexStorage2D(GL_TEXTURE_2D, 1, format, static_cast<gl_21::GLsizei>(width), static_cast<gl_21::GLsizei>(height));
		gl_21::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		gl_21::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gl_21::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		gl_21::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		gl_21::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	// warning OpenGL 3
	gl_21::glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multisampled), id, 0);
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

/*
static gl_21::GLenum FBTextureFormatToGL(FramebufferTextureFormat format) {
	switch (format) {
		case FramebufferTextureFormat::RGBA8:
			return GL_RGBA8;
		case FramebufferTextureFormat::RED_INTEGER:
			return GL_RED;
		case FramebufferTextureFormat::None:
		case FramebufferTextureFormat::DEPTH24STENCIL8:
			break;
	}
	OWL_CORE_ASSERT(false, "Bad Texture format")
	return 0;
}
*/

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
	// todo: OpenGL 3
	gl_21::glDeleteFramebuffers(1, &rendererID);
	gl_21::glDeleteTextures(static_cast<gl_21::GLsizei>(colorAttachments.size()), colorAttachments.data());
	gl_21::glDeleteTextures(1, &depthAttachment);
}

void Framebuffer::invalidate() {

	if (rendererID) {
		// todo: OpenGL 3
		gl_21::glDeleteFramebuffers(1, &rendererID);
		gl_21::glDeleteTextures(static_cast<gl_21::GLsizei>(colorAttachments.size()), colorAttachments.data());
		gl_21::glDeleteTextures(1, &depthAttachment);
		colorAttachments.clear();
		depthAttachment = 0;
	}

	gl_21::glGenFramebuffers(1, &rendererID);
	gl_21::glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

	// todo: OpenGL 3
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
					utils::attachColorTexture(colorAttachments[i], static_cast<int>(specs.samples), GL_R, GL_RED, specs.width, specs.height, static_cast<int>(i));
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
		gl_21::GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		gl_21::glDrawBuffers(static_cast<gl_21::GLsizei>(colorAttachments.size()), buffers);
	} else if (colorAttachments.empty()) {
		// Only depth-pass
		gl_21::glDrawBuffer(GL_NONE);
	}

	// Warning OpenGL 3
	bool completeFramebuffer = gl_21::glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	OWL_CORE_ASSERT(completeFramebuffer, "Framebuffer is incomplete!")
	if (!completeFramebuffer) {
		OWL_CORE_WARN("Incomplete Framebuffer")
	}

	gl_21::glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Framebuffer::bind() {
	// todo: OpenGL 3
	gl_21::glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
	gl_21::glViewport(0, 0, static_cast<gl_21::GLsizei>(specs.width), static_cast<gl_21::GLsizei>(specs.height));
}

void Framebuffer::unbind() {
	gl_21::glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

int Framebuffer::readPixel(uint32_t attachmentIndex, [[maybe_unused]] int x, [[maybe_unused]] int y) {
	OWL_CORE_ASSERT(attachmentIndex < colorAttachments.size(), "ReadPixel bad attachment index")

	gl_21::glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	int pixelData = 0;
	gl_21::glReadPixels(x, y, 1, 1, GL_RED, GL_INT, &pixelData);
	return pixelData;
}

void Framebuffer::clearAttachment(uint32_t attachmentIndex, [[maybe_unused]] int value) {
	OWL_CORE_ASSERT(attachmentIndex < colorAttachments.size(), "clearAttachment bad attachment index")
	[[maybe_unused]] const auto &spec = colorAttachmentSpecifications[attachmentIndex];
	// todo: OpenGL 4
	//## gl_21::glClearTexImage(colorAttachments[attachmentIndex], 0,
	//## 					   utils::FBTextureFormatToGL(spec.textureFormat), GL_INT, &value);
}

}// namespace owl::renderer::opengl_legacy
