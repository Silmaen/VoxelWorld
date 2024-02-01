/**
 * @file SwapChain.cpp
 * @author Silmaen
 * @date 01/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "SwapChain.h"

#include "core/Application.h"
#include "renderer/vulkan/GraphContext.h"

namespace owl::renderer::vulkan::internal {

bool SwapChain::create(VkDevice logicDevice, const PhysicalDevice &phy) {
	auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());
	device = logicDevice;

	VkSurfaceFormatKHR surfaceFormat = phy.formats.front();
	for (const auto &availableFormat: phy.formats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			surfaceFormat = availableFormat;
		}
	}
	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (const auto &availablePresentMode: phy.presentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = availablePresentMode;
		}
	}
	VkExtent2D extent;
	if (phy.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		extent = phy.capabilities.currentExtent;
	} else {
		auto sizes = core::Application::get().getWindow().getSize();
		extent.width = std::clamp(sizes.width(), phy.capabilities.minImageExtent.width, phy.capabilities.maxImageExtent.width);
		extent.height = std::clamp(sizes.height(), phy.capabilities.minImageExtent.height, phy.capabilities.maxImageExtent.height);
	}

	uint32_t imageCount = phy.capabilities.minImageCount + 1;
	if (phy.capabilities.maxImageCount > 0 && imageCount > phy.capabilities.maxImageCount) {
		imageCount = phy.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = gc->getSurface();

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const auto queueFamilyIndices = phy.queues.getIndices();
	if (queueFamilyIndices.size() > 1) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
		createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = phy.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create swap chain!")
		return false;
	}

	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
	return true;
}

void SwapChain::release() {
	if (swapChain != nullptr) {
		vkDestroySwapchainKHR(device, swapChain, nullptr);
		swapChain = nullptr;
	}
}


}// namespace owl::renderer::vulkan::internal
