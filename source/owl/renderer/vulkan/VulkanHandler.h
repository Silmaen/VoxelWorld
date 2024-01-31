/**
 * @file VulkanHandler.h
 * @author Silmaen
 * @date 30/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <vulkan/vulkan.h>

namespace owl::renderer::vulkan {

/**
 * @brief Class VulkanHandler.
 */
class VulkanHandler {
public:
	VulkanHandler(const VulkanHandler &) = delete;
	VulkanHandler(VulkanHandler &&) = delete;
	VulkanHandler &operator=(const VulkanHandler &) = delete;
	VulkanHandler &operator=(VulkanHandler &&) = delete;

	/**
	 * @brief Destructor.
	 */
	virtual ~VulkanHandler();

	/**
	 * @brief Handler for vulkan objects
	 * @return Vulcan handler
	 */
	static VulkanHandler &get() {
		static VulkanHandler handler;
		return handler;
	}

	/**
	 * @brief Initialize the vulkan handler.
	 */
	void initVulkan();

	/**
	 * @brief Release the vulkan handler.
	 */
	void release();

	/// List of handler states.
	enum struct State {
		/// Not initialized of closed.
		Uninitialized,
		/// Initialized and ready.
		Running,
		/// Encounter an error while creating the instance.
		ErrorCreatingInstance,
		/// Encounter an error while setuping the debug.
		ErrorSetupDebugging,
		/// Encounter an error while enumerating the physical devices.
		ErrorEnumeratingPhysicalDevices,
		/// No compatible GPU found.
		ErrorNoGpuFound,
		ErrorCreatingLogicalDevice,
		ErrorCreatingCommandPool,
	};

	/**
	 * @brief Gets the current state of the handler.
	 * @return The state of the handler.
	 */
	[[nodiscard]] const State &getState() const { return state; }

	/**
	 * @brief The vulkan version.
	 * @return The actual Vulkan version
	 */
	[[nodiscard]] int getVersion() const { return version; }

	/**
	 * @brief Activate the validation layer, if not already initialized.
	 */
	void activateValidation() {
		if (state == State::Uninitialized) validation = true;
	}

private:
	/**
	 * @brief Default Constructor.
	 */
	VulkanHandler();

	/**
	 * @brief Create the instance.
	 */
	void createInstance(bool enableValidation = false);

	void createLogicalDevice();

	void setupDebugging();
	void getEnabledFeatures();
	void getEnabledExtensions();

	/// Vulkan Instance
	VkInstance instance = nullptr;
	/// Vulkan physical device.
	VkPhysicalDevice physicalDevice = nullptr;
	/// Physical device properties (for e.g. checking device limits).
	[[maybe_unused]] VkPhysicalDeviceProperties deviceProperties{};
	/// Features available on the selected physical device (for e.g. checking if a feature is available).
	[[maybe_unused]] VkPhysicalDeviceFeatures deviceFeatures{};
	/// Available memory (type) properties for the physical device
	[[maybe_unused]] VkPhysicalDeviceMemoryProperties deviceMemoryProperties{};
	/// Debug messenger.
	VkDebugUtilsMessengerEXT debugUtilsMessenger{};
	/// Enable Validation layers.
	bool validation = false;
	/// The current state of the handler.
	State state = State::Uninitialized;
	/// The list of supported extensions.
	std::vector<std::string> supportedInstanceExtensions;
	std::vector<std::string> supportedPhysicalDeviceExtensions;
	/// Loaded version.
	int version = 0;
	VkDevice logicalDevice = nullptr;
	VkPhysicalDeviceFeatures enabledFeatures{};
	std::vector<const char *> enabledDeviceExtensions;
	[[maybe_unused]] VkQueue queue = nullptr;
	std::vector<VkQueueFamilyProperties> queueFamilyProperties;
	VkCommandPool commandPool = nullptr;
	struct
	{
		uint32_t graphics = 0;
		uint32_t compute = 0;
		uint32_t transfer = 0;
	} queueFamilyIndices;

	[[nodiscard]] uint32_t getQueueFamilyIndex(VkQueueFlags queueFlags) const;
	void createCommandPool(uint32_t queueFamilyIndex);
};

}// namespace owl::renderer::vulkan
