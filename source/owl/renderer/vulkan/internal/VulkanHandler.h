/**
 * @file VulkanHandler.h
 * @author Silmaen
 * @date 30/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "SwapChain.h"

#include <backends/imgui_impl_vulkan.h>
#include <map>

/**
 * @brief Internal functions of the vulkan renderer.
 */
namespace owl::renderer::vulkan::internal {

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
		ErrorCreatingWindowSurface,
		ErrorCreatingSwapChain,
		ErrorCreatingImagesView,
		ErrorCreatingRenderPass,
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

	[[nodiscard]] ImGui_ImplVulkan_InitInfo toImGuiInfo() const;

	[[nodiscard]] VkRenderPass getRenderPath() const { return renderPass; }

	[[nodiscard]] VkDevice getDevice() const { return logicalDevice; }

	[[nodiscard]] VkPipeline getPipeline(int32_t id) const;
	int32_t pushPpeline(VkPipeline pipeline);
	void popPipeline(int32_t id);

private:
	/**
	 * @brief Default Constructor.
	 */
	VulkanHandler();

	/**
	 * @brief Create the instance.
	 */
	void createInstance();
	void createSurface();
	void createPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createGraphicsPipeLine();
	void createDescriptorPool();

	void setupDebugging();

	/// The current state of the handler.
	State state = State::Uninitialized;
	/// Loaded version.
	int version = 0;
	/// Enable Validation layers.
	bool validation = false;
	/// Validation layers available?.
	bool validationPresent = false;

	/// Vulkan Instance
	VkInstance instance = nullptr;
	/// The list of supported extensions.
	std::vector<std::string> supportedInstanceExtensions;

	/// Debug messenger.
	VkDebugUtilsMessengerEXT debugUtilsMessenger{};

	/// The physical device.
	PhysicalDevice physicalDevice;

	/// The logical device.
	VkDevice logicalDevice = nullptr;
	/// The swapchain.
	SwapChain swapChain;
	/// The images views.
	std::vector<VkImageView> swapChainImageViews;
	/// The render pass.
	VkRenderPass renderPass = nullptr;
	/// Descriptor pool.
	VkDescriptorPool descriptorPool = nullptr;
	/// List of piplines.
	std::map<int32_t, VkPipeline> pipeLines;
};

}// namespace owl::renderer::vulkan::internal
