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


namespace owl::renderer::vulkan {

RenderAPI::~RenderAPI() {
	vkDestroyInstance(instance, nullptr);
}

void RenderAPI::init() {
	OWL_PROFILE_FUNCTION()

	if (getState() != State::Created)
		return;
	auto gc = core::Application::get().getWindow().getGraphContext();
	auto vers = gc->getVersion();
	{
		if (vers <= GraphContext::Version{1, 3}) {
			OWL_CORE_ERROR("Insufficent version found {}.{} need at leaset 1.3", vers.major, vers.minor)
			return;
		}
	}

	{
		auto wnd = static_cast<GLFWwindow *>(core::Application::get().getWindow().getNativeWindow());
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

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


	// renderer is now ready
	setState(State::Ready);
}

void RenderAPI::setViewport(uint32_t, uint32_t, uint32_t, uint32_t) {}

void RenderAPI::setClearColor(const glm::vec4 &) {}

void RenderAPI::clear() {}

void RenderAPI::drawData(const shared<DrawData> &, uint32_t) {}

void RenderAPI::setLineWidth(float) {}


}// namespace owl::renderer::vulkan
