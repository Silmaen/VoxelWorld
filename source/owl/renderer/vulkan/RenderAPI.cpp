/**
 * @file RenderAPI.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "GraphContext.h"
#include "RenderAPI.h"
#include "core/Application.h"
#include "core/external/glfw3.h"


namespace owl::renderer::vulkan {

RenderAPI::~RenderAPI() {
	vkDestroyInstance(instance, nullptr);
}

void RenderAPI::init() {
	OWL_PROFILE_FUNCTION()

	if (getState() != State::Created)
		return;
	auto gc = dynamic_cast<GraphContext *>(core::Application::get().getWindow().getGraphContext());
	auto vers = gc->getVersion();
	{
		if (vers < GraphContext::Version{1, 3}) {
			OWL_CORE_ERROR("Insufficient version found {}.{} need at least 1.3", vers.major, vers.minor)
			return;
		}
	}

	// renderer is now ready
	setState(State::Ready);
}

void RenderAPI::setViewport(uint32_t, uint32_t, uint32_t, uint32_t) {}

void RenderAPI::setClearColor(const glm::vec4 &) {}

void RenderAPI::clear() {}

void RenderAPI::drawData(const shared<DrawData> &, uint32_t) {}

void RenderAPI::setLineWidth(float) {}


}// namespace owl::renderer::vulkan
