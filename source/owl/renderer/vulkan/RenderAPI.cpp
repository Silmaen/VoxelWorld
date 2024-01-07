/**
 * @file RenderAPI.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "RenderAPI.h"


namespace owl::renderer::vulkan {

void RenderAPI::init() {
	OWL_PROFILE_FUNCTION()

	if (getState() != State::Created)
		return;

	// renderer is now ready
	setState(State::Ready);
}

void RenderAPI::setViewport(uint32_t, uint32_t, uint32_t, uint32_t) {}

void RenderAPI::setClearColor(const glm::vec4 &) {}

void RenderAPI::clear() {}

void RenderAPI::drawData(const shared<DrawData> &, uint32_t) {}

void RenderAPI::setLineWidth(float) {}

}// namespace owl::renderer::vulkan
