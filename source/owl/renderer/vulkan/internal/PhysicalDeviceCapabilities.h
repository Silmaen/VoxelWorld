/**
 * @file PhysicalDeviceCapabilities.h
 * @author Silmaen
 * @date 11/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <vulkan/vulkan.h>

namespace owl::renderer::vulkan::internal {

/**
 * @brief Class PhysicalDeviceCapabilities.
 */
class PhysicalDeviceCapabilities final {
public:
	/**
	 * @brief Default Constructor.
	 * @param dev The physical device to analyse.
	 */
	explicit PhysicalDeviceCapabilities(VkPhysicalDevice dev = nullptr);
	/**
	 * @brief Copy Constructor.
	 */
	PhysicalDeviceCapabilities(const PhysicalDeviceCapabilities &) = default;
	/**
	 * @brief Move Constructor.
	 */
	PhysicalDeviceCapabilities(PhysicalDeviceCapabilities &&) = default;
	/**
	 * @brief Copy assignation.
	 */
	PhysicalDeviceCapabilities &operator=(const PhysicalDeviceCapabilities &) = default;
	/**
	 * @brief Move assignation.
	 */
	PhysicalDeviceCapabilities &operator=(PhysicalDeviceCapabilities &&) = default;

	/**
	 * @brief Destructor.
	 */
	~PhysicalDeviceCapabilities();
	/**
	 * @brief compute the device's score.
	 * @return The device's score.
	 */
	[[nodiscard]] uint32_t getScore() const;

	/**
	 * @brief Check if a layer is available on this device.
	 * @param layer The layer name to test.
	 * @return True if available
	 */
	[[nodiscard]] bool hasLayer(const std::string &layer) const;
	/**
	 * @brief Check if an extension is available on this device.
	 * @param extension The extension name to test.
	 * @return True if available
	 */
	[[nodiscard]] bool hasExtension(const std::string &extension) const;
	/**
	 * @brief Check if some layers are available on this device.
	 * @param layers The layer's names to test.
	 * @return True if all available
	 */
	[[nodiscard]] bool hasLayers(const std::vector<std::string> &layers) const;
	/**
	 * @brief Check if some extensions are available on this device.
	 * @param extensions The extension's names to test.
	 * @return True if all available
	 */
	[[nodiscard]] bool hasExtensions(const std::vector<std::string> &extensions) const;


	/**
	 * @brief Force to check for surface changes.
	 */
	void updateSurfaceInformations();

	/// The physical device.
	VkPhysicalDevice device = nullptr;
	/// Available layers.
	std::vector<VkLayerProperties> supportedLayers{};
	/// Available extensions.
	std::vector<VkExtensionProperties> supportedExtensions{};
	/// The physical device properties.
	VkPhysicalDeviceProperties properties{};
	/// Features available on the selected physical device (for e.g. checking if a feature is available).
	VkPhysicalDeviceFeatures features{};
	/// Available memory (type) properties for the physical device
	VkPhysicalDeviceMemoryProperties memoryProperties{};
	/// List of queue families.
	std::vector<VkQueueFamilyProperties> queueFamilies{};
	/// Capability of the physical surface
	VkSurfaceCapabilitiesKHR surfaceCapabilities{};
	/// List of supported surface format.
	std::vector<VkSurfaceFormatKHR> surfaceFormats{};
	/// List of supported presentation modes.
	std::vector<VkPresentModeKHR> presentModes{};

	/// Index of the graphic queue.
	uint32_t graphicQueueIndex = std::numeric_limits<uint32_t>::max();
	/// Index of the present queue.
	uint32_t presentQueueIndex = std::numeric_limits<uint32_t>::max();
};

/**
 * @brief Enumerate the physical devices.
 * @return List of physical devices sorted by decreasing score.
 */
std::vector<PhysicalDeviceCapabilities> enumerateDevices(VkInstance instance);

}// namespace owl::renderer::vulkan::internal
