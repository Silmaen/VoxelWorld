/**
 * @file VulkanCore.h
 * @author Silmaen
 * @date 11/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "PhysicalDeviceCapabilities.h"

#include <vulkan/vulkan.h>

/**
 * @brief Namespace for vulkan intenal functions
 */
namespace owl::renderer::vulkan::internal {

/**
 * @brief Simple struct to gather the vulkan configurations.
 */
struct VulkanConfiguraton {
	/// If the validation layers should be activated.
	bool activeValidation = false;
};

struct InstanceInformations {
	InstanceInformations();
	[[nodiscard]] bool hasMinimalVersion(uint8_t major, uint8_t minor, uint8_t patch = 0) const;
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
	uint32_t version = 0;
	std::vector<std::string> supportedExtensions{};
	std::vector<std::string> supportedLayers{};
};

/**
 * @brief Class VulkanCore.
 */
class VulkanCore final {
public:
	VulkanCore(const VulkanCore &) = delete;
	VulkanCore(VulkanCore &&) = delete;
	VulkanCore &operator=(const VulkanCore &) = delete;
	VulkanCore &operator=(VulkanCore &&) = delete;

	/**
	 * @brief Destructor.
	 */
	~VulkanCore();

	/**
	 * @brief Singleton get instance.
	 * @return Intance of Vulkan core.
	 */
	static VulkanCore &get() {
		static VulkanCore instance;
		return instance;
	}

	/**
	 * @brief Initialize the vulkan core.
	 * @param configuration The Given configuratio.
	 */
	void init(const VulkanConfiguraton &configuration);

	/**
	 * @brief Release the vulkan core.
	 */
	void release();

	/**
	 * @brief Check if the currnt core is in good health.
	 * @return True if everything ok.
	 */
	[[nodiscard]] bool isHealthy() const;

	/**
	 * @brief Access to the vulkan instance.
	 * @return The vulkan instance.
	 */
	[[nodiscard]] VkInstance getInstance() const { return instance; }

	/**
	 * @brief Access to the physical device.
	 * @return The physical device.
	 */
	[[nodiscard]] VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }

	/**
	 * @brief Access to the logical device.
	 * @return The logical device.
	 */
	[[nodiscard]] VkDevice getLogicalDevice() const { return logicalDevice; }

	/**
	 * @brief The different core states
	 */
	enum struct State {
		/// Just created or reseted.
		Created,
		/// Initialized and ready.
		Initialized,
		/// Something is wrong.
		Error
	};

	/**
	 * @brief Access to the actual state of the core.
	 * @return The state of the core.
	 */
	[[nodiscard]] const State &getState() const { return state; }

	/**
	 * @brief Get the graphic queue index.
	 * @return The graph queue index.
	 */
	[[nodiscard]] uint32_t getGraphQueueFamilyIndex() const { return phyProps.graphicQueueIndex; }

	/**
	 * @brief Access to the graphic queue.
	 * @return The graphic queue.
	 */
	[[nodiscard]] VkQueue getGraphicQueue() const { return graphicQueue; }
	/**
	 * @brief Access to the present queue.
	 * @return The present queue.
	 */
	[[nodiscard]] VkQueue getPresentQueue() const { return presentQueue; }
	/**
	 * @brief Get the current Extent.
	 * @return The current Extent.
	 */
	[[nodiscard]] VkExtent2D getCurrentExtent() const;

	/**
	 * @brief Get the surface format.
	 * @return The surface format.
	 */
	[[nodiscard]] VkSurfaceFormatKHR getSurfaceFormat() const;

	/**
	 * @brief Get the present mode.
	 * @return The present mode.
	 */
	[[nodiscard]] VkPresentModeKHR getPresentMode() const;

	/**
	 * @brief Get image count.
	 * @return The image count.
	 */
	[[nodiscard]] uint32_t getImagecount() const;

	/**
	 * @brief Get the current transform.
	 * @return The current transform.
	 */
	[[nodiscard]] VkSurfaceTransformFlagBitsKHR getCurrentTransform() const;

	/**
	 * @brief Get The queue indices.
	 * @return The queue indices.
	 */
	[[nodiscard]] std::vector<uint32_t> getQueueIndicies() const;

	/**
	 * @brief Force to check for surface changes.
	 */
	void updateSurfaceInformations();

	/**
	 * @brief Search for a memory type index.
	 * @param typeFilter Filter on type.
	 * @param memProperties Memory properties.
	 * @return The memory type index.
	 */
	[[nodiscard]] uint32_t findMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags memProperties) const;

private:
	/**
	 * @brief Default Constructor.
	 */
	VulkanCore();

	/// Vulkan requested configuation.
	VulkanConfiguraton config{};

	/// Vulkan instance.
	VkInstance instance = nullptr;
	/// Vulkan selected physical device.
	VkPhysicalDevice physicalDevice = nullptr;
	/// Vulkan logical device.
	VkDevice logicalDevice = nullptr;
	/// Debug messenger.
	VkDebugUtilsMessengerEXT debugUtilsMessenger{};

	/// Information about the instance.
	InstanceInformations instanceInfo{};
	/// If  validation layer are enabled.
	bool hasValidation = false;
	/// The internal state.
	State state = State::Created;
	/// Save of the current physical device properties.
	PhysicalDeviceCapabilities phyProps;

	/// The graphic queue.
	VkQueue graphicQueue = nullptr;
	/// The present queue.
	VkQueue presentQueue = nullptr;

	void createInstance();
	void selectPhysicalDevice();
	void createLogicalDevice();
	void setupDebugging();
	void createQueues();
};

}// namespace owl::renderer::vulkan::internal
