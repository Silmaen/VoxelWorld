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

namespace owl::renderer::opengl {


Framebuffer::Framebuffer(const FramebufferSpecification &spec): specs{spec} {
	invalidate();
}
Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &rendererID);
}
void Framebuffer::invalidate() {
	glCreateFramebuffers(1, &rendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

	glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
	glBindTexture(GL_TEXTURE_2D, colorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specs.width, specs.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
	glBindTexture(GL_TEXTURE_2D, depthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, specs.width, specs.height);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, specs.width, specs.height, 0,
	// 	GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

	OWL_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Framebuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
}
void Framebuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}// namespace owl::renderer::opengl
