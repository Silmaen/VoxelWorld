/**
 * @file VulkanCore.cpp
 * @author Silmaen
 * @date 11/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "VulkanCore.h"

#include "core/Application.h"
#include "renderer/vulkan/GraphContext.h"
#include "utils.h"

namespace owl::renderer::vulkan::internal {

namespace {

constexpr VkApplicationInfo appInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = nullptr,
		.pApplicationName = "Owl",
		.applicationVersion = {},
		.pEngineName = "OwlEngine",
		.engineVersion = {},
		.apiVersion = VK_API_VERSION_1_3};

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

VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessagerCI{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.pNext = nullptr,
		.flags = {},
		.messageSeverity = 0x1111,
		.messageType = 0x1111,
		.pfnUserCallback = debugUtilsMessageCallback,
		.pUserData = nullptr};

PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;

}// namespace

// ============= VulkanCore =====================

VulkanCore::VulkanCore() = default;

VulkanCore::~VulkanCore() = default;

void VulkanCore::init(const VulkanConfiguraton &configuration) {
	config = configuration;
	if (!instanceInfo.hasMinimalVersion(1, 3)) {
		OWL_CORE_ERROR("Vulkan: cannot initialize du to insuficient instance version. Require 1.3")
		state = State::Error;
		return;
	}
	createInstance();
	if (state == State::Error)
		return;
	if (hasValidation) {
		setupDebugging();
		if (state == State::Error)
			return;
	}
	const auto gc = dynamic_cast<GraphContext *>(core::Application::get().getWindow().getGraphContext());
	if (const VkResult result = gc->createSurface(instance); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create window surface ({})", resultString(result))
		state = State::Error;
		return;
	}
	selectPhysicalDevice();
	if (state == State::Error)
		return;
	createLogicalDevice();
	createQueues();
	if (isHealthy())
		state = State::Initialized;
}

void VulkanCore::release() {
	if (logicalDevice != nullptr) {
		vkDestroyDevice(logicalDevice, nullptr);
		logicalDevice = nullptr;
		OWL_CORE_TRACE("Vulkan: logicalDevice destroyed.")
	}
	{
		const auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());
		gc->destroySurface(instance);
		OWL_CORE_TRACE("Vulkan: Surface destroyed.")
	}
	if (debugUtilsMessenger != nullptr) {
		vkDestroyDebugUtilsMessengerEXT(instance, debugUtilsMessenger, nullptr);
		debugUtilsMessenger = nullptr;
		OWL_CORE_TRACE("Vulkan: debugUtilsMessenger destroyed.")
	}
	vkDestroyInstance(instance, nullptr);
	instance = nullptr;
	OWL_CORE_TRACE("Vulkan: instance destroyed.")
}

void VulkanCore::createInstance() {
	// first check for requested Layers
	std::vector<std::string> requestedLayers;
	if (config.activeValidation) {
		if (!instanceInfo.hasLayer("VK_LAYER_KHRONOS_validation")) {
			OWL_CORE_WARN("Vulkan: Missing validation layers, go one without it.")
		} else {
			requestedLayers.emplace_back("VK_LAYER_KHRONOS_validation");
			hasValidation = true;
		}
	}
	if (!instanceInfo.hasLayers(requestedLayers)) {
		OWL_CORE_ERROR("Vulkan: Missing mandatory instance layers.")
		state = State::Error;
		return;
	}
	// second check for requested Extensions.
	std::vector<std::string> requestedExtensions;
	{
		std::set<const char *> uniqueExtensions;
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		uniqueExtensions.insert(glfwExtensions, glfwExtensions + glfwExtensionCount);
		uniqueExtensions.emplace(VK_KHR_SURFACE_EXTENSION_NAME);
		if (hasValidation) {
			uniqueExtensions.emplace(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		requestedExtensions.assign(uniqueExtensions.begin(), uniqueExtensions.end());
	}
	if (!instanceInfo.hasExtensions(requestedExtensions)) {
		OWL_CORE_ERROR("Vulkan: Missing mandatory instance extensions.")
		state = State::Error;
		return;
	}
	// now creation.
	std::vector<const char *> layers;
	for (const auto &layer: requestedLayers)
		layers.emplace_back(layer.c_str());
	std::vector<const char *> extensions;
	for (const auto &extension: requestedExtensions)
		extensions.emplace_back(extension.c_str());

	VkInstanceCreateInfo instanceCI{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = hasValidation ? &debugUtilsMessagerCI : nullptr,
			.flags = {},
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = static_cast<uint32_t>(layers.size()),
			.ppEnabledLayerNames = layers.data(),
			.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
			.ppEnabledExtensionNames = extensions.data()};
	VkResult result = vkCreateInstance(&instanceCI, nullptr, &instance);
	bool second = false;
	if (result == VK_ERROR_LAYER_NOT_PRESENT) {
		OWL_CORE_ERROR("Vulkan: Unable to create instance because a layer can't be loaded, Retry without layers.")
		second = true;
		instanceCI.ppEnabledLayerNames = nullptr;
		instanceCI.enabledLayerCount = 0;
		result = vkCreateInstance(&instanceCI, nullptr, &instance);
	}
	if (result != VK_SUCCESS) {
		if (second)
			OWL_CORE_ERROR("Vulkan: Definitively unable to create instance ({}).", resultString(result))
		else
			OWL_CORE_ERROR("Vulkan: Unable to create instance ({}).", resultString(result))
		state = State::Error;
	}
}

bool VulkanCore::isHealthy() const {
	return instance != nullptr &&
		   physicalDevice != nullptr &&
		   logicalDevice != nullptr &&
		   graphicQueue != nullptr &&
		   presentQueue != nullptr;
}

void VulkanCore::selectPhysicalDevice() {
	const auto devices = enumerateDevices(instance);
	if (devices.empty()) {
		OWL_CORE_ERROR("Vulkan: No suitable physical device found.")
		state = State::Error;
		return;
	}
	phyProps = devices.front();
	if (phyProps.getScore() == 0) {
		OWL_CORE_ERROR("Vulkan: No suitable physical device found, scores 0.")
		state = State::Error;
		return;
	}
	physicalDevice = phyProps.device;
}

void VulkanCore::createLogicalDevice() {
	std::vector<VkDeviceQueueCreateInfo> deviceQueuesCI;
	float queuePriority = 1.0f;
	for (const uint32_t queue: std::set{phyProps.graphicQueueIndex, phyProps.presentQueueIndex}) {
		VkDeviceQueueCreateInfo deviceQueueCI{
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.pNext = nullptr,
				.flags = {},
				.queueFamilyIndex = queue,
				.queueCount = 1,
				.pQueuePriorities = &queuePriority};
		deviceQueuesCI.push_back(deviceQueueCI);
	}
	const std::vector layerNames = {"VK_LAYER_KHRONOS_validation"};
	const std::vector extensionNames = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	const VkDeviceCreateInfo deviceCI{
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.queueCreateInfoCount = static_cast<uint32_t>(deviceQueuesCI.size()),
			.pQueueCreateInfos = deviceQueuesCI.data(),
			.enabledLayerCount = hasValidation ? static_cast<uint32_t>(extensionNames.size()) : static_cast<uint32_t>(extensionNames.size()) - 1,
			.ppEnabledLayerNames = layerNames.data(),
			.enabledExtensionCount = static_cast<uint32_t>(extensionNames.size()),
			.ppEnabledExtensionNames = extensionNames.data(),
			.pEnabledFeatures = nullptr};
	if (const VkResult result = vkCreateDevice(physicalDevice, &deviceCI, nullptr, &logicalDevice); result != VK_SUCCESS) {
		state = State::Error;
		OWL_CORE_ERROR("Vulkan: Error while creating the logical device ({})", resultString(result))
		return;
	}
}

void VulkanCore::setupDebugging() {
	OWL_DIAG_PUSH
	OWL_DIAG_DISABLE_CLANG17("-Wcast-function-type-strict")
	vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
	vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
	OWL_DIAG_POP
	if (const VkResult result = vkCreateDebugUtilsMessengerEXT(instance, &debugUtilsMessagerCI, nullptr, &debugUtilsMessenger); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: Error while setup debugging ({})", resultString(result))
		state = State::Error;
	}
}

void VulkanCore::createQueues() {
	vkGetDeviceQueue(logicalDevice, phyProps.graphicQueueIndex, 0, &graphicQueue);
	vkGetDeviceQueue(logicalDevice, phyProps.presentQueueIndex, 0, &presentQueue);
}

VkExtent2D VulkanCore::getCurrentExtent() const {
	VkExtent2D extent;
	if (phyProps.surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		extent = phyProps.surfaceCapabilities.currentExtent;
	} else {
		auto sizes = core::Application::get().getWindow().getSize();
		extent.width = std::clamp(sizes.width(), phyProps.surfaceCapabilities.minImageExtent.width, phyProps.surfaceCapabilities.maxImageExtent.width);
		extent.height = std::clamp(sizes.height(), phyProps.surfaceCapabilities.minImageExtent.height, phyProps.surfaceCapabilities.maxImageExtent.height);
	}
	return extent;
}

VkSurfaceFormatKHR VulkanCore::getSurfaceFormat() const {
	VkSurfaceFormatKHR surfaceFormat = phyProps.surfaceFormats.front();
	for (const auto &availableFormat: phyProps.surfaceFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			surfaceFormat = availableFormat;
		}
	}
	return surfaceFormat;
}

VkPresentModeKHR VulkanCore::getPresentMode() const {
	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (const auto &availablePresentMode: phyProps.presentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = availablePresentMode;
		}
	}
	return presentMode;
}

uint32_t VulkanCore::getImagecount() const {
	uint32_t imageCount = phyProps.surfaceCapabilities.minImageCount + 1;
	if (phyProps.surfaceCapabilities.maxImageCount > 0 && imageCount > phyProps.surfaceCapabilities.maxImageCount) {
		imageCount = phyProps.surfaceCapabilities.maxImageCount;
	}
	return imageCount;
}

VkSurfaceTransformFlagBitsKHR VulkanCore::getCurrentTransform() const {
	return phyProps.surfaceCapabilities.currentTransform;
}

std::vector<uint32_t> VulkanCore::getQueueIndicies() const {
	return {phyProps.graphicQueueIndex, phyProps.presentQueueIndex};
}

void VulkanCore::updateSurfaceInformations() {
	phyProps.updateSurfaceInformations();
}

uint32_t VulkanCore::findMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags memProperties) const {
	for (uint32_t i = 0; i < phyProps.memoryProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (phyProps.memoryProperties.memoryTypes[i].propertyFlags & memProperties) == memProperties) {
			return i;
		}
	}
	OWL_CORE_ERROR("Vulkan PhysicalDevice: failed to find suitable memory type!")
	return std::numeric_limits<uint32_t>::max();
}

// ============= VulkanCore =====================

// ============= InstanceInformations =====================

InstanceInformations::InstanceInformations() {
	vkEnumerateInstanceVersion(&version);
	OWL_CORE_INFO("Vulkan: Found API Version: {}.{}.", VK_API_VERSION_MAJOR(version), VK_API_VERSION_MINOR(version))
	// Get the list of available layers
	{
		supportedLayers.clear();
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> Layers(layerCount);
		if (const VkResult result = vkEnumerateInstanceLayerProperties(&layerCount, Layers.data()); result == VK_SUCCESS) {
			for (const auto &[layerName, specVersion, implementationVersion, description]: Layers) {
				supportedLayers.emplace_back(layerName);
				OWL_CORE_TRACE("Vulkan: Instance layer: {} version {} // {}", layerName, specVersion, description)
			}
		} else {
			OWL_CORE_WARN("Vulkan: unable to enumerate instance layers ({}).", resultString(result))
		}
	}
	// Get extensions supported by the instance and store for later use
	{
		supportedExtensions.clear();
		uint32_t extCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
		if (extCount > 0) {
			std::vector<VkExtensionProperties> extensions(extCount);
			if (const VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()); result == VK_SUCCESS) {
				for (const auto &[extensionName, specVersion]: extensions) {
					supportedExtensions.emplace_back(extensionName);
					OWL_CORE_TRACE("Vulkan: Supported instance extension: {} version: {}", extensionName, specVersion)
				}
			} else {
				OWL_CORE_WARN("Vulkan: unable to enumerate instance extensions ({}).", resultString(result))
			}
		}
	}
}

bool InstanceInformations::hasMinimalVersion(const uint8_t major, const uint8_t minor, const uint8_t patch) const {
	return VK_API_VERSION_MAJOR(version) > major ||
		   (VK_API_VERSION_MAJOR(version) == major && VK_API_VERSION_MINOR(version) > minor) ||
		   (VK_API_VERSION_MAJOR(version) == major && VK_API_VERSION_MINOR(version) == minor && VK_API_VERSION_PATCH(version) >= patch);
}

bool InstanceInformations::hasLayer(const std::string &layer) const {
	return std::ranges::find(supportedLayers.begin(), supportedLayers.end(), layer) != supportedLayers.end();
}

bool InstanceInformations::hasExtension(const std::string &extension) const {
	return std::ranges::find(supportedExtensions.begin(), supportedExtensions.end(), extension) != supportedExtensions.end();
}

bool InstanceInformations::hasLayers(const std::vector<std::string> &layers) const {
	return std::ranges::all_of(layers.begin(), layers.end(), [&](const auto &layer) { return this->hasLayer(layer); });
}

bool InstanceInformations::hasExtensions(const std::vector<std::string> &extensions) const {
	return std::ranges::all_of(extensions.begin(), extensions.end(), [&](const auto &extension) { return this->hasExtension(extension); });
}

// ============= InstanceInformations =====================

}// namespace owl::renderer::vulkan::internal
