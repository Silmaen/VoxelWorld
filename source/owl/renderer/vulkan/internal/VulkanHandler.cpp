/**
 * @file VulkanHandler.cpp
 * @author Silmaen
 * @date 30/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "VulkanHandler.h"

#include "../GraphContext.h"
#include "core/Application.h"

namespace owl::renderer::vulkan::internal {

namespace {

PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;

VkBool32 debugUtilsMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
								   VkDebugUtilsMessageTypeFlagsEXT,
								   const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
								   void *) {

	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
		OWL_CORE_TRACE("Vulkan: [{}][{}]: {}", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage)
	} else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
		OWL_CORE_INFO("Vulkan: [{}][{}]: {}", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage)
	} else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		OWL_CORE_WARN("Vulkan: [{}][{}]: {}", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage)
	} else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		OWL_CORE_ERROR("Vulkan: [{}][{}]: {}", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage)
	}
	// The return value of this callback controls whether the Vulkan call that caused the validation message will be aborted or not
	// We return VK_FALSE as we DON'T want Vulkan calls that cause a validation message to abort
	// If you instead want to have calls abort, pass in VK_TRUE and the function will return VK_ERROR_VALIDATION_FAILED_EXT
	return false;
}

}// namespace

VulkanHandler::VulkanHandler() = default;

VulkanHandler::~VulkanHandler() = default;

void VulkanHandler::initVulkan() {
	createInstance();
	if (state == State::ErrorCreatingInstance)
		return;
	OWL_CORE_TRACE("Vulkan: Instance created.")
	if (validation) {
		setupDebugging();
		if (state == State::ErrorSetupDebugging) return;
		OWL_CORE_TRACE("Vulkan: Debug support initiated.")
	}
	createSurface();
	if (state != State::Uninitialized)
		return;
	OWL_CORE_TRACE("Vulkan: Surface created.")
	createPhysicalDevice();
	if (state != State::Uninitialized)
		return;
	OWL_CORE_TRACE("Vulkan: Physical Device created.")
	createLogicalDevice();
	if (state != State::Uninitialized)
		return;
	OWL_CORE_TRACE("Vulkan: Logical Device created.")
	createSwapChain();
	if (state != State::Uninitialized)
		return;
	OWL_CORE_TRACE("Vulkan: Swap Chain created.")
	state = State::Running;
}

void VulkanHandler::release() {
	if (instance == nullptr) return;// nothing can exists without instance.
	swapChain.release();

	if (logicalDevice != nullptr) {
		vkDestroyDevice(logicalDevice, nullptr);
		logicalDevice = nullptr;
	}
	{
		const auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());
		gc->destroySurface(instance);
	}
	if (debugUtilsMessenger != nullptr) {
		vkDestroyDebugUtilsMessengerEXT(instance, debugUtilsMessenger, nullptr);
		debugUtilsMessenger = nullptr;
	}
	vkDestroyInstance(instance, nullptr);
	instance = nullptr;
	state = State::Uninitialized;
}

static void func(VkResult) {}

ImGui_ImplVulkan_InitInfo VulkanHandler::toImGuiInfo() const {
	return {
			.Instance = instance,
			.PhysicalDevice = physicalDevice.device,
			.Device = logicalDevice,
			.QueueFamily = physicalDevice.queues.graphicsIndex,
			.Queue = physicalDevice.queues.graphics,
			.PipelineCache = {},
			.DescriptorPool = {},
			.Subpass = 0,
			.MinImageCount = 2,
			.ImageCount = 2,
			.MSAASamples = VK_SAMPLE_COUNT_1_BIT,
			.UseDynamicRendering = false,
			.ColorAttachmentFormat = {},
			.Allocator = nullptr,
			.CheckVkResultFn = func,
			.MinAllocationSize = 1024 * 1024};
}

void VulkanHandler::createInstance() {

	uint32_t instanceVersion;
	vkEnumerateInstanceVersion(&instanceVersion);
	version = static_cast<int>(instanceVersion & 0xFFFFF000);//Remove the patch version.
	OWL_CORE_INFO("Vulkan: Found API Version: {}.{}.", VK_API_VERSION_MAJOR(version), VK_API_VERSION_MINOR(version))

	// App info
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Owl";
	appInfo.pEngineName = "OwlEngine";
	appInfo.apiVersion = VK_API_VERSION_1_3;

	// Supported Instance extensions & Layer
	{
		// Get extensions supported by the instance and store for later use
		uint32_t extCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
		if (extCount > 0) {
			std::vector<VkExtensionProperties> extensions(extCount);
			if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS) {
				for (VkExtensionProperties &extension: extensions) {
					supportedInstanceExtensions.emplace_back(extension.extensionName);
					OWL_CORE_TRACE("Vulkan: Supported instance extension: {}", extension.extensionName)
				}
			}
		}
	}

	// extension for the instance creation.
	std::vector<const char *> instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME};
	// Include Extension from glfw.
	{
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::set<const char *> uniqueExtensions(instanceExtensions.begin(), instanceExtensions.end());
		uniqueExtensions.insert(glfwExtensions, glfwExtensions + glfwExtensionCount);
		instanceExtensions.assign(uniqueExtensions.begin(), uniqueExtensions.end());
	}
	if (validation && std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == supportedInstanceExtensions.end()) {
		OWL_CORE_WARN("Vulkan: request validation layer but is not supported, deactivating.")
		validation = false;
	}

	// create Info
	VkInstanceCreateInfo instanceCI{};
	instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCI.pApplicationInfo = &appInfo;
	// potentially include Debug Messager
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessagerCI{};
	if (validation) {
		debugUtilsMessagerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugUtilsMessagerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugUtilsMessagerCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		debugUtilsMessagerCI.pfnUserCallback = debugUtilsMessageCallback;
		debugUtilsMessagerCI.pNext = instanceCI.pNext;
		instanceCI.pNext = &debugUtilsMessagerCI;
		instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	// Give the list of required extention to the Instance Create Info
	instanceCI.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	instanceCI.ppEnabledExtensionNames = instanceExtensions.data();

	if (validation) {
		const char *validationLayerName = "VK_LAYER_KHRONOS_validation";
		// Check if this layer is available at instance level
		uint32_t instanceLayerCount;
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
		std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
		OWL_CORE_TRACE("Vulkan: Found {} instance layer.", instanceLayerCount)
		validationPresent = false;
		for (const VkLayerProperties &layer: instanceLayerProperties) {
			OWL_CORE_TRACE("Vulkan: Instance layer: {}", layer.layerName)
			if (strcmp(layer.layerName, validationLayerName) == 0) {
				validationPresent = true;
				break;
			}
		}
		if (validationPresent) {
			instanceCI.ppEnabledLayerNames = &validationLayerName;
			instanceCI.enabledLayerCount = 1;
		} else {
			OWL_CORE_WARN("Vulkan: Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled")
		}
	}

	if (vkCreateInstance(&instanceCI, nullptr, &instance) != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: Unable to create instance.")
		state = State::ErrorCreatingInstance;
	}
}

void VulkanHandler::setupDebugging() {
	OWL_DIAG_PUSH
	OWL_DIAG_DISABLE_CLANG17("-Wcast-function-type-strict")
	vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
	vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
	OWL_DIAG_POP
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessagerCI{};
	debugUtilsMessagerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsMessagerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugUtilsMessagerCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
	debugUtilsMessagerCI.pfnUserCallback = debugUtilsMessageCallback;
	if (vkCreateDebugUtilsMessengerEXT(instance, &debugUtilsMessagerCI, nullptr, &debugUtilsMessenger) != VK_SUCCESS)
		state = State::ErrorSetupDebugging;
}

void VulkanHandler::createSurface() {
	auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());
	if (gc->createSurface(instance) != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create window surface!")
		state = State::ErrorCreatingWindowSurface;
	}
}

void VulkanHandler::createPhysicalDevice() {
	uint32_t deviceCount = 0;
	{
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			OWL_CORE_ERROR("Vulkan: Cannot find any GPUs with vulkan support.")
			state = State::ErrorNoGpuFound;
			return;
		}
	}
	bool found = false;
	{
		physicalDevice.score = 0;
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		for (const auto &device: devices) {
			PhysicalDevice dev;
			dev.create(device);
			OWL_CORE_TRACE("Vulkan: found device: {} @ api {}", dev.getName(), dev.getVersonStr())
			for (const auto &layer: dev.layers) {
				OWL_CORE_TRACE("        layer       : {} {}", layer.layerName, layer.description)
			}
			if (dev.score > physicalDevice.score) {
				physicalDevice = std::move(dev);
				found = true;
			}
		}
	}
	if (!found) {
		OWL_CORE_ERROR("Vulkan: Found no graphics card with enough capabilities.")
		state = State::ErrorNoGpuFound;
		return;
	}
	OWL_CORE_INFO("Vulkan: Using GPU: {}.", physicalDevice.getName())
}

void VulkanHandler::createLogicalDevice() {
	std::vector<VkDeviceQueueCreateInfo> deviceQueuesCI;
	for (const uint32_t queue: physicalDevice.queues.getUniqueIndices()) {
		VkDeviceQueueCreateInfo deviceQueueCI{};
		deviceQueueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCI.queueCount = 1;
		deviceQueueCI.queueFamilyIndex = queue;
		float queuePriority = 1.0f;
		deviceQueueCI.pQueuePriorities = &queuePriority;
		deviceQueuesCI.push_back(deviceQueueCI);
	}
	VkDeviceCreateInfo deviceCI{};
	deviceCI.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCI.pQueueCreateInfos = deviceQueuesCI.data();
	deviceCI.queueCreateInfoCount = static_cast<uint32_t>(deviceQueuesCI.size());

	deviceCI.pEnabledFeatures = &physicalDevice.enabledFeatures;

	deviceCI.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	deviceCI.ppEnabledExtensionNames = deviceExtensions.data();

	if (validationPresent) {
		const char *validationLayerName = "VK_LAYER_KHRONOS_validation";
		deviceCI.enabledLayerCount = 1;
		deviceCI.ppEnabledLayerNames = &validationLayerName;
	} else {
		deviceCI.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice.device, &deviceCI, nullptr, &logicalDevice) != VK_SUCCESS) {
		state = State::ErrorCreatingLogicalDevice;
		return;
	}

	physicalDevice.queues.defineQueues(logicalDevice);
}

void VulkanHandler::createSwapChain() {
	if (!swapChain.create(logicalDevice, physicalDevice))
		state = State::ErrorCreatingSwapChain;
}

}// namespace owl::renderer::vulkan::internal
