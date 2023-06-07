/**
 * @file RenderAPI.cpp
 * @author Silmaen
 * @date 09/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "RenderAPI.h"

#include "gl_46/glad.h"

namespace owl::renderer::opengl {

[[maybe_unused]] static void messageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, [[maybe_unused]] int length, const char *message, const void *userParam) {
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			OWL_CORE_CRITICAL("OpenGL: {}({})-{} : {} / {}", source, type, id, message, userParam)
			return;
		case GL_DEBUG_SEVERITY_MEDIUM:
			OWL_CORE_ERROR("OpenGL: {}({})-{} : {} / {}", source, type, id, message, userParam)
			return;
		case GL_DEBUG_SEVERITY_LOW:
			OWL_CORE_WARN("OpenGL: {}({})-{} : {} / {}", source, type, id, message, userParam)
			return;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			OWL_CORE_TRACE("OpenGL: {}({})-{} : {} / {}", source, type, id, message, userParam)
			return;
	}

	OWL_CORE_ASSERT(false, "Unknown severity level!")
}

void RenderAPI::init() {
	OWL_PROFILE_FUNCTION()

	bool goodVersion = gl_46::GLVersion.major > 4 || (gl_46::GLVersion.major == 4 && gl_46::GLVersion.minor >= 5);
	if (!goodVersion) {
		setState(State::Error);
		OWL_CORE_ERROR("Owl Engine OpenGL Renderer requires at least OpenGL version 4.5 but version {}.{} found", gl_46::GLVersion.major, gl_46::GLVersion.minor)
	}

	if (getState() != State::Created)
		return;
#ifdef OWL_DEBUG
	gl_46::glEnable(GL_DEBUG_OUTPUT);
	gl_46::glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	gl_46::glDebugMessageCallback(messageCallback, nullptr);

	gl_46::glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

	gl_46::glEnable(GL_BLEND);
	gl_46::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gl_46::glEnable(GL_DEPTH_TEST);
	gl_46::glEnable(GL_LINE_SMOOTH);

	// renderer is now ready
	setState(State::Ready);
}

void RenderAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	gl_46::glViewport(static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(width), static_cast<int32_t>(height));
}

void RenderAPI::setClearColor(const glm::vec4 &color) {
	gl_46::glClearColor(color.r, color.g, color.b, color.a);
}

void RenderAPI::clear() {
	gl_46::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderAPI::drawIndexed(const shared<VertexArray> &vertexArray, uint32_t indexCount) {
	vertexArray->bind();
	uint32_t count = indexCount ? indexCount : vertexArray->getIndexBuffer()->getCount();
	gl_46::glDrawElements(GL_TRIANGLES, static_cast<int32_t>(count), GL_UNSIGNED_INT, nullptr);
}

void RenderAPI::drawLines(const shared<VertexArray> &vertexArray, uint32_t vertexCount) {
	vertexArray->bind();
	gl_46::glDrawArrays(GL_LINES, 0, static_cast<gl_46::GLsizei>(vertexCount));
}

void RenderAPI::setLineWidth(float width) {
	gl_46::glLineWidth(width);
}

}// namespace owl::renderer::opengl
