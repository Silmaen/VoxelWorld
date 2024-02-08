/**
 * @file RenderAPI.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "RenderAPI.h"

#include "core/Application.h"
#include "core/external/glfw3.h"
#include "internal/VulkanHandler.h"


namespace owl::renderer::vulkan {

RenderAPI::~RenderAPI() {
	auto &vkh = internal::VulkanHandler::get();
	vkh.release();
}

void RenderAPI::init() {
	OWL_PROFILE_FUNCTION()

	if (getState() != State::Created) return;

	auto &vkh = internal::VulkanHandler::get();
#ifdef OWL_DEBUG
	vkh.activateValidation();
#endif
	vkh.initVulkan();
	if (vkh.getState() != internal::VulkanHandler::State::Running) {
		setState(State::Error);
		return;
	}

	// renderer is now ready
	setState(State::Ready);
}

void RenderAPI::setViewport(uint32_t, uint32_t, uint32_t, uint32_t) {
	auto &vkh = internal::VulkanHandler::get();
	vkh.setResize();
}

void RenderAPI::setClearColor(const glm::vec4 &col) {
	auto &vkh = internal::VulkanHandler::get();
	VkClearValue val;
	val.color.float32[0] = col.r;
	val.color.float32[1] = col.g;
	val.color.float32[2] = col.b;
	val.color.float32[3] = col.a;
	vkh.setClearColor(val);
}

void RenderAPI::clear() {}

void RenderAPI::drawData(const shared<DrawData> &data, uint32_t index) {
	const auto &vkh = internal::VulkanHandler::get();
	data->bind();
	const uint32_t count = index ? index : data->getIndexCount();
	vkh.drawData(count);
}

void RenderAPI::setLineWidth(float) {}

void RenderAPI::beginFrame() {
	auto &vkh = internal::VulkanHandler::get();
	if (vkh.getState() != internal::VulkanHandler::State::Running) {
		OWL_CORE_ERROR("Vulkan is in error state ({}).", magic_enum::enum_name(vkh.getState()))
		setState(State::Error);
		return;
	}
	vkh.beginFrame();
}

void RenderAPI::endFrame() {
	auto &vkh = internal::VulkanHandler::get();
	if (vkh.getState() != internal::VulkanHandler::State::Running) {
		OWL_CORE_ERROR("Vulkan is in error state: ({}).", magic_enum::enum_name(vkh.getState()))
		setState(State::Error);
		return;
	}
	vkh.endFrame();
}

}// namespace owl::renderer::vulkan
