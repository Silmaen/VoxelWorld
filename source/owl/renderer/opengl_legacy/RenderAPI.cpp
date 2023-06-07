/**
 * @file RenderAPI.cpp
 * @author Silmaen
 * @date 30/06/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Manager.h"
#include "RenderAPI.h"

#include "gl_21/glad.h"

namespace owl::renderer::opengl_legacy {

void RenderAPI::init() {
	OWL_PROFILE_FUNCTION()

	bool goodVersion = gl_21::GLVersion.major > 2 || (gl_21::GLVersion.major == 2 && gl_21::GLVersion.minor >= 1);
	if (!goodVersion) {
		setState(State::Error);
		OWL_CORE_ERROR("Owl Engine OpenGL Renderer requires at least OpenGL version 2.1 but version {}.{} found",
					   gl_21::GLVersion.major, gl_21::GLVersion.minor)
	}

	if (getState() != State::Created)
		return;

	Manager::get().init();

	// renderer is now ready
	setState(State::Ready);
}

void RenderAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	gl_21::glViewport(static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(width), static_cast<int32_t>(height));
}

void RenderAPI::setClearColor(const glm::vec4 &color) {
	gl_21::glClearColor(color.r, color.g, color.b, color.a);
}

void RenderAPI::clear() {
	gl_21::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderAPI::drawIndexed(const shared<renderer::VertexArray> &vertexArray, uint32_t indexCount) {
	OWL_PROFILE_FUNCTION()

	if (getState() != State::Ready)
		return;
	Manager::get().drawIndexed(vertexArray, indexCount);
}

void RenderAPI::drawLines(const shared<renderer::VertexArray> &vertexArray, uint32_t vertexCount) {
	OWL_PROFILE_FUNCTION()

	Manager::get().drawLines(vertexArray, vertexCount);
}

void RenderAPI::setLineWidth(float width) {
	OWL_PROFILE_FUNCTION()

	gl_21::glLineWidth(width);
}

}// namespace owl::renderer::opengl_legacy
