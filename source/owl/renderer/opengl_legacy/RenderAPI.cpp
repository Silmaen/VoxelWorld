/**
 * @file RenderAPI.cpp
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "RenderAPI.h"

#include "core/Application.h"
#include "core/external/opengl21.h"

namespace owl::renderer::opengl_legacy {

void RenderAPI::init() {
	OWL_PROFILE_FUNCTION()

	const auto vers = core::Application::get().getWindow().getGraphContext()->getVersion();

	if (const bool goodVersion = vers.major > 2 || (vers.major == 2 && vers.minor >= 1); !goodVersion) {
		setState(State::Error);
		OWL_CORE_ERROR("Owl Engine legacy OpenGL Renderer requires at least OpenGL version 2.1 but version {}.{} found",
					   vers.major, vers.minor)
	}


	if (getState() != State::Created)
		return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);

	// renderer is now ready
	setState(State::Ready);
}

void RenderAPI::setViewport(const uint32_t iX, const uint32_t iY, const uint32_t iWidth, const uint32_t iHeight) {
	glViewport(static_cast<int32_t>(iX), static_cast<int32_t>(iY), static_cast<int32_t>(iWidth),
			   static_cast<int32_t>(iHeight));
}

void RenderAPI::setClearColor(const glm::vec4 &iColor) { glClearColor(iColor.r, iColor.g, iColor.b, iColor.a); }

void RenderAPI::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void RenderAPI::drawData(const shared<DrawData> &iData, const uint32_t iIndexCount) {
	iData->bind();
	const uint32_t count = iIndexCount ? iIndexCount : iData->getIndexCount();
	glDrawElements(GL_TRIANGLES, static_cast<int32_t>(count), GL_UNSIGNED_INT, nullptr);
	iData->unbind();
}

void RenderAPI::setLineWidth(const float iWidth) { glLineWidth(iWidth); }

uint32_t RenderAPI::getMaxTextureSlots() const {
	int32_t textureUnits = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
	return std::min(static_cast<uint32_t>(textureUnits), 16u);
}

}// namespace owl::renderer::opengl_legacy
