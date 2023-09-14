/**
 * @file RenderAPI.cpp
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "RenderAPI.h"

#include "glad21/glad.h"

namespace owl::renderer::opengl_legacy {


void RenderAPI::init() {
	OWL_PROFILE_FUNCTION()

	bool goodVersion = GLVersion.major > 2 || (GLVersion.major == 2 && GLVersion.minor >= 1);
	if (!goodVersion) {
		setState(State::Error);
		OWL_CORE_ERROR("Owl Engine legacy OpenGL Renderer requires at least OpenGL version 2.1 but version {}.{} found", GLVersion.major, GLVersion.minor)
	}


	if (getState() != State::Created) return;

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
	data->unbind();
}

void RenderAPI::setLineWidth(float width) {
	glLineWidth(width);
}

uint32_t RenderAPI::getMaxTextureSlots() const {
	int32_t textureUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
	return std::min(static_cast<uint32_t>(textureUnits), 16u);
}

}// namespace owl::renderer::opengl_legacy
