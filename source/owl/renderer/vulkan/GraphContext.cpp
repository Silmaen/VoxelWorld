/**
 * @file GraphContext.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */


#include "GraphContext.h"

#include "internal/VulkanHandler.h"

namespace owl::renderer::vulkan {

GraphContext::GraphContext(GLFWwindow *window)
	: wnd(window){};

GraphContext::~GraphContext() = default;

void GraphContext::swapBuffers() {
	auto &vkh = internal::VulkanHandler::get();
	vkh.swapFrame();
}

void GraphContext::init() {
}

GraphContext::Version GraphContext::getVersion() const {
	int version = internal::VulkanHandler::get().getVersion();
	return {static_cast<int32_t>(VK_API_VERSION_MAJOR(version)), static_cast<int32_t>(VK_API_VERSION_MAJOR(version))};
}

VkResult GraphContext::createSurface(const VkInstance instance) {
	return glfwCreateWindowSurface(instance, wnd, nullptr, &surface);
}


void GraphContext::destroySurface(const VkInstance instance) {
	vkDestroySurfaceKHR(instance, surface, nullptr);
	surface = nullptr;
}

void GraphContext::waitIdle() {
	OWL_CORE_TRACE("GraphContext Wait for Idle.")
	auto &vkh = internal::VulkanCore::get();
	vkDeviceWaitIdle(vkh.getLogicalDevice());
}

}// namespace owl::renderer::vulkan
