/**
 * @file PhysicalDevice.h
 * @author Silmaen
 * @date 01/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <vulkan/vulkan.h>

namespace owl::renderer::vulkan::internal {

static const std::vector<const char *> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME};

/**
 * \brief Class queues
 */
struct Queues {
	/// Index of the graph queue for this device.
	uint32_t graphicsIndex = 0;
	/// The graphics queue.
	VkQueue graphics = nullptr;
	/// Index of the graph queue for this device.
	uint32_t presentIndex = 0;
	/// The graphics queue.
	VkQueue present = nullptr;

	void defineQueues(const VkDevice &device);
	[[nodiscard]] std::vector<uint32_t> getIndices() const;
	[[nodiscard]] std::set<uint32_t> getUniqueIndices() const;
};

/**
 * @brief Class PhysicalDevice.
 */
struct PhysicalDevice {
	/// Vulkan physical device.
	VkPhysicalDevice device = nullptr;
	/// Physical device properties (for e.g. checking device limits).
	VkPhysicalDeviceProperties properties{};
	/// Features available on the selected physical device (for e.g. checking if a feature is available).
	VkPhysicalDeviceFeatures features{};
	/// Available memory (type) properties for the physical device
	VkPhysicalDeviceMemoryProperties memoryProperties{};
	/// Neabled device features.
	VkPhysicalDeviceFeatures enabledFeatures{};
	/// Capability of the physical surface
	VkSurfaceCapabilitiesKHR capabilities{};
	/// List of supported surface format.
	std::vector<VkSurfaceFormatKHR> formats{};
	/// List of supported presentation modes.
	std::vector<VkPresentModeKHR> presentModes{};

	/// List of queue families.
	std::vector<VkQueueFamilyProperties> queueFamilies{};
	/// Available layers.
	std::vector<VkLayerProperties> layers{};
	/// Available extensions.
	std::vector<VkExtensionProperties> extensions{};

	/// Queue details.
	Queues queues;

	/// Vulkan Physiscal device actual score.
	double score = 0;

	void create(VkPhysicalDevice dev);

	[[nodiscard]] bool checkExtensionSupport() const;
	void updateSurfaceInformations();

	[[nodiscard]] std::string getName() const;
	[[nodiscard]] std::string getVersonStr() const;
	[[nodiscard]] uint32_t findMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags memProperties) const;
};

}// namespace owl::renderer::vulkan::internal
