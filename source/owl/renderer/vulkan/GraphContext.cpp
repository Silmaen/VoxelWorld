/**
 * @file GraphContext.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */


#include "GraphContext.h"

#include "VulkanHandler.h"

namespace owl::renderer::vulkan {

GraphContext::GraphContext(GLFWwindow *window)
	: wnd(window){};

GraphContext::~GraphContext() = default;

void GraphContext::swapBuffers() {}

void GraphContext::init() {


#if false
	glfwGetInstanceProcAddress(instance, "vkGetDeviceProcAddr");
	err = glfwCreateWindowSurface(instance, wnd, nullptr, &surface);
	if (err) {
		OWL_CORE_ERROR("Unable to initialize Vulkan GraphContext, error ({}) {}.", static_cast<int>(err), magic_enum::enum_name<VkResult>(err))
		return;
	}
#endif
}

GraphContext::Version GraphContext::getVersion() const {
	int version = VulkanHandler::get().getVersion();
	return {static_cast<int32_t>(VK_API_VERSION_MAJOR(version)), static_cast<int32_t>(VK_API_VERSION_MAJOR(version))};
}

}// namespace owl::renderer::vulkan
