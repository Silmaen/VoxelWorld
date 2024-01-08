/**
 * @file GraphContext.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */


#include "GraphContext.h"

namespace owl::renderer::vulkan {

GraphContext::GraphContext(GLFWwindow *window) : wnd(window){};

GraphContext::~GraphContext() {
	vkDestroyInstance(instance, nullptr);
}

void GraphContext::swapBuffers() {}

void GraphContext::init() {

	uint32_t instanceVersion;
	vkEnumerateInstanceVersion(&instanceVersion);
	version = static_cast<int>(instanceVersion & 0xFFFFF000);//Remove the patch version.

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Owl Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Owl Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char **glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;

	VkResult err = vkCreateInstance(&createInfo, nullptr, &instance);
	if (err) {
		OWL_CORE_ERROR("Unable to create Vulkan Instance, error ({}) {}.", static_cast<int>(err), magic_enum::enum_name<VkResult>(err))
		return;
	}

	glfwGetInstanceProcAddress(instance, "vkGetDeviceProcAddr");
	err = glfwCreateWindowSurface(instance, wnd, nullptr, &surface);
	if (err) {
		OWL_CORE_ERROR("Unable to initialize Vulkan GraphContext, error ({}) {}.", static_cast<int>(err), magic_enum::enum_name<VkResult>(err))
		return;
	}
}

GraphContext::Version GraphContext::getVersion() const {
	return {static_cast<int32_t>(VK_API_VERSION_MAJOR(version)), static_cast<int32_t>(VK_API_VERSION_MAJOR(version))};
}

}// namespace owl::renderer::vulkan
