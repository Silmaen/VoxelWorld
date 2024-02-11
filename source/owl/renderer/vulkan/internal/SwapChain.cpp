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
#include "utils.h"

namespace owl::renderer::vulkan::internal {

void SwapChain::create(const VkDevice &logicDevice, const PhysicalDevice &physicalDevice) {
	device = logicDevice;
	phy = &physicalDevice;

	VkExtent2D extent;
	if (phy->capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		extent = phy->capabilities.currentExtent;
	} else {
		auto sizes = core::Application::get().getWindow().getSize();
		extent.width = std::clamp(sizes.width(), phy->capabilities.minImageExtent.width, phy->capabilities.maxImageExtent.width);
		extent.height = std::clamp(sizes.height(), phy->capabilities.minImageExtent.height, phy->capabilities.maxImageExtent.height);
	}

	createSwapChain(extent);
	if (state != State::Created)
		return;
	OWL_CORE_TRACE("  Internal swapchain created.")
	createImageViews();
	if (state != State::Created)
		return;
	OWL_CORE_TRACE("  Internal swapchain imageviews created.")
	createRenderPass();
	if (state != State::Created)
		return;
	OWL_CORE_TRACE("  Internal swapchain renderpass created.")
	createSwapChainFrameBuffers();
	if (state != State::Created)
		return;
	OWL_CORE_TRACE("  Internal swapchain framebuffers created.")

	state = State::Initialized;
}

void SwapChain::cleanup() {
	for (const auto &framebuffer: swapChainFramebuffers) {
		if (framebuffer != nullptr)
			vkDestroyFramebuffer(device, framebuffer, nullptr);
	}
	swapChainFramebuffers.clear();
	for (const auto &imageView: swapChainImageViews) {
		if (imageView != nullptr)
			vkDestroyImageView(device, imageView, nullptr);
	}
	swapChainImageViews.clear();
	swapChainImages.clear();
	if (swapChain != nullptr) {
		vkDestroySwapchainKHR(device, swapChain, nullptr);
		swapChain = nullptr;
	}
}

void SwapChain::release() {
	cleanup();
	if (renderPass != nullptr) {
		vkDestroyRenderPass(device, renderPass, nullptr);
		renderPass = nullptr;
	}
	state = State::Created;
}

void SwapChain::createSwapChain(const VkExtent2D &extent) {
	auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());

	VkSurfaceFormatKHR surfaceFormat = phy->formats.front();
	for (const auto &availableFormat: phy->formats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			surfaceFormat = availableFormat;
		}
	}
	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (const auto &availablePresentMode: phy->presentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = availablePresentMode;
		}
	}

	uint32_t imageCount = phy->capabilities.minImageCount + 1;
	if (phy->capabilities.maxImageCount > 0 && imageCount > phy->capabilities.maxImageCount) {
		imageCount = phy->capabilities.maxImageCount;
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

	const auto queueFamilyIndices = phy->queues.getIndices();
	if (queueFamilyIndices.size() > 1) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
		createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = phy->capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	const VkResult result = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain);
	if (result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create swap chain ({}).", resultString(result))
		state = State::ErrorCreatingSwapChain;
		return;
	}

	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

void SwapChain::createImageViews() {
	swapChainImageViews.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		const VkResult result = vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]);
		if (result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan SwapChain: Error creating image views ({}).", resultString(result))
			state = State::ErrorCreatingImagesView;
		}
	}
}

void SwapChain::createRenderPass() {
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	const VkResult result = vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass);
	if (result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create render pass ({})", resultString(result))
		state = State::ErrorCreatingRenderPass;
	}
}

void SwapChain::createSwapChainFrameBuffers() {
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		VkImageView attachments[] = {swapChainImageViews[i]};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;
		const VkResult result = vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]);
		if (result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan SwapChain: Error creating image views ({}).", resultString(result))
			state = State::ErrorCreatingFramebuffer;
		}
	}
}

void SwapChain::recreate(VkExtent2D extent) {

	if (extent.width == swapChainExtent.width && extent.height == swapChainExtent.height)
		return;

	vkDeviceWaitIdle(device);

	cleanup();

	createSwapChain(extent);
	createImageViews();
	createSwapChainFrameBuffers();
}

}// namespace owl::renderer::vulkan::internal
