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

struct SwapChain {
	VkSwapchainKHR swapChain = nullptr;
	std::vector<VkImage> swapChainImages{};
	VkFormat swapChainImageFormat{VK_FORMAT_UNDEFINED};
	VkExtent2D swapChainExtent{};
	VkDevice device{nullptr};

	bool create(VkDevice logicDevice, const PhysicalDevice &phy);
	void release();
};

}// namespace owl::renderer::vulkan::internal
