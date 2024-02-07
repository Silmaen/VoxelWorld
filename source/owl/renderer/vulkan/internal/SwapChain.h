/**
 * @file SwapChain.h
 * @author Silmaen
 * @date 01/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "PhysicalDevice.h"

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

	void create(const VkDevice &logicDevice, const PhysicalDevice &physicalDevice);
	void release();

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
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createSwapChainFrameBuffers();
	VkDevice device{nullptr};
	const PhysicalDevice *phy{nullptr};
};

}// namespace owl::renderer::vulkan::internal
