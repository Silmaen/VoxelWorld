/**
 * @file RenderAPI.cpp
 * @author Silmaen
 * @date 09/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "RenderAPI.h"
#include "core/Application.h"
#include "core/external/opengl46.h"

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

	auto vers = core::Application::get().getWindow().getGraphContext()->getVersion();

	bool goodVersion = vers.major > 4 || (vers.major == 4 && vers.minor >= 5);
	if (!goodVersion) {
		setState(State::Error);
		OWL_CORE_ERROR("Owl Engine OpenGL Renderer requires at least OpenGL version 4.5 but version {}.{} found", vers.major, vers.minor)
	}

	if (getState() != State::Created)
		return;
#ifdef OWL_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(messageCallback, nullptr);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);

	// renderer is now ready
	setState(State::Ready);
}

void RenderAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	glViewport(static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(width), static_cast<int32_t>(height));
}

void RenderAPI::setClearColor(const glm::vec4 &color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void RenderAPI::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderAPI::drawData(const shared<DrawData> &data, uint32_t indexCount) {
	data->bind();
	uint32_t count = indexCount ? indexCount : data->getIndexCount();
	glDrawElements(GL_TRIANGLES, static_cast<int32_t>(count), GL_UNSIGNED_INT, nullptr);
}

void RenderAPI::setLineWidth(float width) {
	glLineWidth(width);
}

uint32_t RenderAPI::getMaxTextureSlots() const {
	int32_t textureUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
	return std::min(32u, static_cast<uint32_t>(textureUnits));
}

}// namespace owl::renderer::opengl
