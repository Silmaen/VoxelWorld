/**
 * @file VulkanHandler.cpp
 * @author Silmaen
 * @date 30/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "VulkanHandler.h"

#include <GLFW/glfw3.h>

namespace owl::renderer::vulkan {

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

VulkanHandler::~VulkanHandler() {
	release();
}

void VulkanHandler::initVulkan() {
	createInstance(validation);
	if (state == State::ErrorCreatingInstance) return;
	if (validation) {
		setupDebugging();
		if (state == State::ErrorSetupDebugging) return;
	}
	return;
	/*
	// Physical device
	uint32_t gpuCount = 0;
	{
		// Get number of available physical devices
		if (vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr) != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: Error while enumerating the physical devices.")
			state = State::ErrorEnumeratingPhysicalDevices;
			return;
		}
		if (gpuCount == 0) {
			OWL_CORE_ERROR("Vulkan: No device with Vulkan support found")
			state = State::ErrorNoGpuFound;
			return;
		}

		// Enumerate devices
		std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
		if (vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data()) != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: Error while enumerating the physical devices.")
			state = State::ErrorEnumeratingPhysicalDevices;
			return;
		}

		// GPU selection
		// Select physical device to be used for the Vulkan example
		// Defaults to the first device unless specified by command line
		const uint32_t selectedDevice = 0;
		// Todo (next version): GPU selection by different means.
		physicalDevice = physicalDevices[selectedDevice];
	}

	// Store properties (including limits), features and memory properties of the physical device (so that examples can check against them)
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);
	OWL_CORE_INFO("Vulkan: Using device: {}", deviceProperties.deviceName)

	// Derived examples can override this to set actual features (based on above readings) to enable for logical device creation
	getEnabledFeatures();

	// Vulkan device creation
	// This is handled by a separate class that gets a logical device representation
	// and encapsulates functions related to a device
	{
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		assert(queueFamilyCount > 0);
		queueFamilyProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

		// Get list of supported extensions
		uint32_t extCount = 0;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, nullptr);
		if (extCount > 0) {
			std::vector<VkExtensionProperties> extensions(extCount);
			if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, &extensions.front()) == VK_SUCCESS) {
				for (auto ext: extensions) {
					supportedPhysicalDeviceExtensions.emplace_back(ext.extensionName);
				}
			}
		}
	}

	// Derived examples can enable extensions based on the list of supported extensions read from the physical device
	getEnabledExtensions();

	createLogicalDevice();
	if (state != State::Uninitialized) {
		return;
	}

	// Get a graphics queue from the device
	vkGetDeviceQueue(logicalDevice, queueFamilyIndices.graphics, 0, &queue);

	// Finaly set the state.
	//state = State::Running;
	*/
}

void VulkanHandler::release() {

	if (commandPool != nullptr)
		vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
	if (logicalDevice != nullptr)
		vkDestroyDevice(logicalDevice, nullptr);
	if (debugUtilsMessenger != nullptr)
		vkDestroyDebugUtilsMessengerEXT(instance, debugUtilsMessenger, nullptr);
	if (instance != nullptr)
		vkDestroyInstance(instance, nullptr);

	state = State::Uninitialized;
}

void VulkanHandler::createInstance(const bool enableValidation) {

	uint32_t instanceVersion;
	vkEnumerateInstanceVersion(&instanceVersion);
	version = static_cast<int>(instanceVersion & 0xFFFFF000);//Remove the patch version.

	validation = enableValidation;

	// App info
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Owl";
	appInfo.pEngineName = "OwlEngine";
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Supported extensions
	{
		// Get extensions supported by the instance and store for later use
		uint32_t extCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
		if (extCount > 0) {
			std::vector<VkExtensionProperties> extensions(extCount);
			if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS) {
				for (VkExtensionProperties &extension: extensions) {
					supportedInstanceExtensions.emplace_back(extension.extensionName);
					OWL_CORE_TRACE("Vulkan instance extension: {}", extension.extensionName)
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
	for (const auto &extension: instanceExtensions) {
		OWL_CORE_TRACE("Vulkan required extensions: {}", extension)
	}

	if (validation) {
		const char *validationLayerName = "VK_LAYER_KHRONOS_validation";
		// Check if this layer is available at instance level
		uint32_t instanceLayerCount;
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
		std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
		bool validationLayerPresent = false;
		for (const VkLayerProperties &layer: instanceLayerProperties) {
			if (strcmp(layer.layerName, validationLayerName) == 0) {
				validationLayerPresent = true;
				break;
			}
		}
		if (validationLayerPresent) {
			instanceCI.ppEnabledLayerNames = &validationLayerName;
			instanceCI.enabledLayerCount = 1;
		} else {
			OWL_CORE_WARN("Vulkan: Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled")
			validation = false;
		}
	}

	const VkResult result = vkCreateInstance(&instanceCI, nullptr, &instance);

	if (result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: Unable to create instance.")
		state = State::ErrorCreatingInstance;
	}
}

void VulkanHandler::setupDebugging() {
	OWL_DIAG_PUSH
	OWL_DIAG_DISABLE_CLANG("-Wcast-function-type-strict")
	vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
	vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
	OWL_DIAG_POP
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessagerCI{};
	debugUtilsMessagerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsMessagerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugUtilsMessagerCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
	debugUtilsMessagerCI.pfnUserCallback = debugUtilsMessageCallback;
	const VkResult result = vkCreateDebugUtilsMessengerEXT(instance, &debugUtilsMessagerCI, nullptr, &debugUtilsMessenger);
	if (result != VK_SUCCESS)
		state = State::ErrorSetupDebugging;
}

void VulkanHandler::getEnabledFeatures() {}

void VulkanHandler::getEnabledExtensions() {}

void VulkanHandler::createLogicalDevice() {
	constexpr VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT & VK_QUEUE_COMPUTE_BIT;
	constexpr bool useSwapChain = true;
	// Desired queues need to be requested upon logical device creation
	// Due to differing queue family configurations of Vulkan implementations this can be a bit tricky, especially if the application
	// requests different queue types

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

	// Get queue family indices for the requested queue family types
	// Note that the indices may overlap depending on the implementation

	constexpr float defaultQueuePriority(0.0f);

	// Graphics queue
	if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT) {
		queueFamilyIndices.graphics = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = queueFamilyIndices.graphics;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &defaultQueuePriority;
		queueCreateInfos.push_back(queueInfo);
	} else {
		queueFamilyIndices.graphics = 0;
	}

	// Dedicated compute queue
	if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT) {
		queueFamilyIndices.compute = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
		if (queueFamilyIndices.compute != queueFamilyIndices.graphics) {
			// If compute family index differs, we need an additional queue create info for the compute queue
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = queueFamilyIndices.compute;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(queueInfo);
		}
	} else {
		// Else we use the same queue
		queueFamilyIndices.compute = queueFamilyIndices.graphics;
	}

	// Dedicated transfer queue
	if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT) {
		queueFamilyIndices.transfer = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
		if ((queueFamilyIndices.transfer != queueFamilyIndices.graphics) && (queueFamilyIndices.transfer != queueFamilyIndices.compute)) {
			// If transfer family index differs, we need an additional queue create info for the transfer queue
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = queueFamilyIndices.transfer;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(queueInfo);
		}
	} else {
		// Else we use the same queue
		queueFamilyIndices.transfer = queueFamilyIndices.graphics;
	}

	// Create the logical device representation
	std::vector<const char *> deviceExtensions(enabledDeviceExtensions);
	if (useSwapChain) {
		// If the device will be used for presenting to a display via a swapchain we need to request the swapchain extension
		deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

	// If a pNext(Chain) has been passed, we need to add it to the device creation info
	/*VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{};
	if (pNextChain) {
		physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		physicalDeviceFeatures2.features = enabledFeatures;
		physicalDeviceFeatures2.pNext = pNextChain;
		deviceCreateInfo.pEnabledFeatures = nullptr;
		deviceCreateInfo.pNext = &physicalDeviceFeatures2;
	}*/

	if (!deviceExtensions.empty()) {
		for (const char *enabledExtension: deviceExtensions) {
			if (std::find(supportedPhysicalDeviceExtensions.begin(), supportedPhysicalDeviceExtensions.end(), enabledExtension) == supportedPhysicalDeviceExtensions.end()) {
				OWL_CORE_ERROR("Vukan: Enabled device extension \"{}\" is not present at device level", enabledExtension)
			}
		}

		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	}

	const VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);
	if (result != VK_SUCCESS) {
		state = State::ErrorCreatingLogicalDevice;
		return;
	}

	// Create a default command pool for graphics command buffers
	createCommandPool(queueFamilyIndices.graphics);
}

uint32_t VulkanHandler::getQueueFamilyIndex(VkQueueFlags queueFlags) const {
	// Dedicated queue for compute
	// Try to find a queue family index that supports compute but not graphics
	if ((queueFlags & VK_QUEUE_COMPUTE_BIT) == queueFlags) {
		for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++) {
			if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
				return i;
			}
		}
	}

	// Dedicated queue for transfer
	// Try to find a queue family index that supports transfer but not graphics and compute
	if ((queueFlags & VK_QUEUE_TRANSFER_BIT) == queueFlags) {
		for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++) {
			if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)) {
				return i;
			}
		}
	}

	// For other queue types or if no separate compute queue is present, return the first one to support the requested flags
	for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++) {
		if ((queueFamilyProperties[i].queueFlags & queueFlags) == queueFlags) {
			return i;
		}
	}

	throw std::runtime_error("Could not find a matching queue family index");
}

void VulkanHandler::createCommandPool(uint32_t queueFamilyIndex) {
	VkCommandPoolCreateInfo cmdPoolInfo = {};
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	if (vkCreateCommandPool(logicalDevice, &cmdPoolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		state = State::ErrorCreatingCommandPool;
	}
}

}// namespace owl::renderer::vulkan
