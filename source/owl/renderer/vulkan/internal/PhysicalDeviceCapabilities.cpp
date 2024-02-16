/**
 * @file PhysicalDeviceCapabilities.cpp
 * @author Silmaen
 * @date 11/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "PhysicalDeviceCapabilities.h"

#include "core/Application.h"
#include "renderer/vulkan/GraphContext.h"
#include "utils.h"

namespace owl::renderer::vulkan::internal {

PhysicalDeviceCapabilities::PhysicalDeviceCapabilities(const VkPhysicalDevice dev) : device(dev) {
	if (device == nullptr)
		return;
	vkGetPhysicalDeviceProperties(device, &properties);
	vkGetPhysicalDeviceFeatures(device, &features);
	vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);
	{
		uint32_t count = 0;
		vkEnumerateDeviceLayerProperties(device, &count, nullptr);
		supportedLayers.resize(count);
		vkEnumerateDeviceLayerProperties(device, &count, supportedLayers.data());
	}
	{
		uint32_t count = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
		supportedExtensions.resize(count);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &count, supportedExtensions.data());
	}
	{
		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
		queueFamilies.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueFamilies.data());
	}
	if (hasExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)) {
		updateSurfaceInformations();
		uint32_t index = 0;
		const auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());
		for (const auto &qFam: queueFamilies) {
			if (qFam.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				graphicQueueIndex = index;
			}
			VkBool32 support;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, index, gc->getSurface(), &support);
			if (support) {
				presentQueueIndex = index;
			}
			index++;
		}
	}
}

PhysicalDeviceCapabilities::~PhysicalDeviceCapabilities() = default;

bool PhysicalDeviceCapabilities::hasLayer(const std::string &layer) const {
	return std::ranges::find_if(supportedLayers.begin(), supportedLayers.end(),
								[&layer](const VkLayerProperties &layerProp) {
									return layerProp.layerName == layer;
								}) != supportedLayers.end();
}

bool PhysicalDeviceCapabilities::hasExtension(const std::string &extension) const {
	return std::ranges::find_if(supportedExtensions.begin(), supportedExtensions.end(),
								[&extension](const VkExtensionProperties &ext) {
									return ext.extensionName == extension;
								}) != supportedExtensions.end();
}

bool PhysicalDeviceCapabilities::hasLayers(const std::vector<std::string> &layers) const {
	return std::ranges::all_of(layers.begin(), layers.end(), [&](const auto &layer) { return this->hasLayer(layer); });
}

bool PhysicalDeviceCapabilities::hasExtensions(const std::vector<std::string> &extensions) const {
	return std::ranges::all_of(extensions.begin(), extensions.end(), [&](const auto &extension) { return this->hasExtension(extension); });
}

uint32_t PhysicalDeviceCapabilities::getScore() const {
	if (!features.geometryShader)
		return 0;
	if (surfaceFormats.empty() || presentModes.empty())
		return 0;
	uint32_t score = 0;
	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;// favorize graphic card over CPU-integrated GPU
	score += properties.limits.maxImageDimension2D;
	// checking the queue families
	if (graphicQueueIndex == std::numeric_limits<uint32_t>::max())
		score *= 0;
	return score;
}

std::vector<PhysicalDeviceCapabilities> enumerateDevices(const VkInstance instance) {
	std::vector<PhysicalDeviceCapabilities> resultVec;
	uint32_t deviceCount = 0;
	{
		if (const VkResult result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: Error while enumerating physical devices ({}).", resultString(result))
			return {};
		}
		if (deviceCount == 0) {
			OWL_CORE_ERROR("Vulkan: Cannot find any GPUs with vulkan support.")
			return {};
		}
	}
	std::vector<VkPhysicalDevice> devices(deviceCount);
	if (const VkResult result = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: Error while enumerating physical devices ({}).", resultString(result))
		return {};
	}
	resultVec.reserve(devices.size());
	for (const auto &device: devices) {
		resultVec.emplace_back(device);
	}
	// sort by decreasing score...
	if (!resultVec.empty())
		std::ranges::sort(resultVec.begin(), resultVec.cend(),
						  [](const PhysicalDeviceCapabilities &a, const PhysicalDeviceCapabilities &b) {
							  return a.getScore() > b.getScore();
						  });
	return resultVec;
}

void PhysicalDeviceCapabilities::updateSurfaceInformations() {
	const auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, gc->getSurface(), &surfaceCapabilities);
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, gc->getSurface(), &formatCount, nullptr);
	if (formatCount != 0) {
		surfaceFormats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, gc->getSurface(), &formatCount, surfaceFormats.data());
	}
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, gc->getSurface(), &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, gc->getSurface(), &presentModeCount, presentModes.data());
	}
}
}// namespace owl::renderer::vulkan::internal
