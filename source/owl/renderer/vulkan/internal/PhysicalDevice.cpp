/**
 * @file PhysicalDevice.cpp
 * @author Silmaen
 * @date 01/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "PhysicalDevice.h"

#include "../GraphContext.h"
#include "core/Application.h"

namespace owl::renderer::vulkan::internal {


void Queues::defineQueues(const VkDevice &device) {
	vkGetDeviceQueue(device, graphicsIndex, 0, &graphics);
	vkGetDeviceQueue(device, presentIndex, 0, &present);
}

std::vector<uint32_t> Queues::getIndices() const {
	return {graphicsIndex, presentIndex};
}
std::set<uint32_t> Queues::getUniqueIndices() const {
	return {graphicsIndex, presentIndex};
}

void PhysicalDevice::create(VkPhysicalDevice dev) {
	// filling informations
	device = dev;
	vkGetPhysicalDeviceProperties(device, &properties);
	vkGetPhysicalDeviceFeatures(device, &features);
	vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);
	{
		uint32_t count;
		vkEnumerateDeviceLayerProperties(device, &count, nullptr);
		layers.resize(count);
		vkEnumerateDeviceLayerProperties(device, &count, layers.data());
	}
	{
		uint32_t count;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
		extensions.resize(count);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extensions.data());
	}
	{
		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
		queueFamilies.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueFamilies.data());
	}
	updateSurfaceInformations();
	// Compute score
	score = 0;
	if (!features.geometryShader)// required !
		return;
	// checking the surface capabilities
	if (formats.empty() || presentModes.empty())
		return;
	// base score.
	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;
	score += properties.limits.maxImageDimension2D;
	// checking the queue families
	{
		double scoreFactor = 0.0;
		uint32_t index = 0;
		for (const auto &qFamily: queueFamilies) {
			if (qFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				scoreFactor = 1.0;
				queues.graphicsIndex = index;
			}
			VkBool32 support;
			const auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());
			vkGetPhysicalDeviceSurfaceSupportKHR(device, index, gc->getSurface(), &support);
			if (support) {
				queues.presentIndex = index;
			}
			index++;
		}
		score *= scoreFactor;
	}
}

bool PhysicalDevice::checkExtensionSupport() const {
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto &extension: extensions)
		requiredExtensions.erase(extension.extensionName);
	return requiredExtensions.empty();
}

std::string PhysicalDevice::getName() const {
	return properties.deviceName;
}

std::string PhysicalDevice::getVersonStr() const {
	return fmt::format("{}.{}.{}.{}", VK_API_VERSION_MAJOR(properties.apiVersion), VK_API_VERSION_MINOR(properties.apiVersion), VK_API_VERSION_PATCH(properties.apiVersion), VK_API_VERSION_VARIANT(properties.apiVersion));
}

void PhysicalDevice::updateSurfaceInformations() {
	if (checkExtensionSupport()) {
		const auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, gc->getSurface(), &capabilities);
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, gc->getSurface(), &formatCount, nullptr);
		if (formatCount != 0) {
			formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, gc->getSurface(), &formatCount, formats.data());
		}
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, gc->getSurface(), &presentModeCount, nullptr);
		if (presentModeCount != 0) {
			presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, gc->getSurface(), &presentModeCount, presentModes.data());
		}
	}
}
uint32_t PhysicalDevice::findMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags memProperties) const {

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & memProperties) == memProperties) {
			return i;
		}
	}
	OWL_CORE_ERROR("Vulkan PhysicalDevice: failed to find suitable memory type!")
	return std::numeric_limits<uint32_t>::max();
}

}// namespace owl::renderer::vulkan::internal
