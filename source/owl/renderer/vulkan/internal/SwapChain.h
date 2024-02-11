/**
 * @file SwapChain.h
 * @author Silmaen
 * @date 01/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <vulkan/vulkan.h>

namespace owl::renderer::vulkan::internal {

/**
 * @brief Vulkan swapchain.
 */
struct SwapChain {
	VkSwapchainKHR swapChain = nullptr;
	std::vector<VkImage> swapChainImages{};
	VkFormat swapChainImageFormat{VK_FORMAT_UNDEFINED};
	VkExtent2D swapChainExtent{};
	std::vector<VkImageView> swapChainImageViews{};
	std::vector<VkFramebuffer> swapChainFramebuffers{};
	/// The render pass.
	VkRenderPass renderPass = nullptr;

	void create(const VkDevice &logicDevice, const VkPhysicalDevice &physicalDevice);
	void release();
	void recreate(VkExtent2D extent);

	enum struct State {
		Created,
		Initialized,
		ErrorCreatingSwapChain,
		ErrorCreatingImagesView,
		ErrorCreatingRenderPass,
		ErrorCreatingFramebuffer,
	};
	State state = State::Created;

private:
	void cleanup();
	void createSwapChain(const VkExtent2D &extent);
	void createImageViews();
	void createRenderPass();
	void createSwapChainFrameBuffers();
	VkDevice device{nullptr};
	VkPhysicalDevice phy{nullptr};
};

}// namespace owl::renderer::vulkan::internal
