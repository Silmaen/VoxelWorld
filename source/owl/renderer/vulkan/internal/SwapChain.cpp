/**
 * @file SwapChain.cpp
 * @author Silmaen
 * @date 01/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "SwapChain.h"

#include "VulkanCore.h"
#include "core/Application.h"
#include "renderer/vulkan/GraphContext.h"
#include "utils.h"

namespace owl::renderer::vulkan::internal {

void SwapChain::create(const VkDevice &logicDevice, const VkPhysicalDevice &physicalDevice) {
	device = logicDevice;
	phy = physicalDevice;

	const auto &core = VulkanCore::get();
	const VkExtent2D extent = core.getCurrentExtent();

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
	const auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());
	const auto &core = VulkanCore::get();
	uint32_t imageCount = core.getImagecount();
	const auto queueFamilyIndices = core.getQueueIndicies();
	const bool shares = queueFamilyIndices.size() > 1;
	const VkSwapchainCreateInfoKHR createInfo{
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.pNext = nullptr,
			.flags = {},
			.surface = gc->getSurface(),
			.minImageCount = imageCount,
			.imageFormat = core.getSurfaceFormat().format,
			.imageColorSpace = core.getSurfaceFormat().colorSpace,
			.imageExtent = extent,
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.imageSharingMode = shares ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = shares ? static_cast<uint32_t>(queueFamilyIndices.size()) : 0,
			.pQueueFamilyIndices = shares ? queueFamilyIndices.data() : nullptr,
			.preTransform = core.getCurrentTransform(),
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = core.getPresentMode(),
			.clipped = VK_TRUE,
			.oldSwapchain = VK_NULL_HANDLE};

	if (const VkResult result = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create swap chain ({}).", resultString(result))
		state = State::ErrorCreatingSwapChain;
		return;
	}
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = core.getSurfaceFormat().format;
	swapChainExtent = extent;
}

void SwapChain::createImageViews() {
	swapChainImageViews.resize(swapChainImages.size());
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = nullptr,
				.flags = {},
				.image = swapChainImages[i],
				.viewType = VK_IMAGE_VIEW_TYPE_3D,
				.format = swapChainImageFormat,
				.components = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY},
				.subresourceRange = {
						.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
						.baseMipLevel = 0,
						.levelCount = 1,
						.baseArrayLayer = 0,
						.layerCount = 1}};
		if (const VkResult result = vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan SwapChain: Error creating image views ({}).", resultString(result))
			state = State::ErrorCreatingImagesView;
		}
	}
}

void SwapChain::createRenderPass() {
	VkAttachmentDescription colorAttachment{
			.flags = {},
			.format = swapChainImageFormat,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR};
	VkAttachmentReference colorAttachmentRef{
			.attachment = 0,
			.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
	const VkSubpassDescription subpass{
			.flags = {},
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.inputAttachmentCount = 0,
			.pInputAttachments = nullptr,
			.colorAttachmentCount = 1,
			.pColorAttachments = &colorAttachmentRef,
			.pResolveAttachments = nullptr,
			.pDepthStencilAttachment = nullptr,
			.preserveAttachmentCount = 0,
			.pPreserveAttachments = nullptr};
	constexpr VkSubpassDependency dependency{
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,
			.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			.dependencyFlags = {}};
	const VkRenderPassCreateInfo renderPassInfo{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.attachmentCount = 1,
			.pAttachments = &colorAttachment,
			.subpassCount = 1,
			.pSubpasses = &subpass,
			.dependencyCount = 1,
			.pDependencies = &dependency};

	if (const VkResult result = vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create render pass ({})", resultString(result))
		state = State::ErrorCreatingRenderPass;
	}
}

void SwapChain::createSwapChainFrameBuffers() {
	swapChainFramebuffers.resize(swapChainImageViews.size());
	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		const VkImageView attachments[] = {swapChainImageViews[i]};
		VkFramebufferCreateInfo framebufferInfo{
				.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.pNext = nullptr,
				.flags = {},
				.renderPass = renderPass,
				.attachmentCount = 1,
				.pAttachments = attachments,
				.width = swapChainExtent.width,
				.height = swapChainExtent.height,
				.layers = 1};

		if (const VkResult result = vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan SwapChain: Error creating image views ({}).", resultString(result))
			state = State::ErrorCreatingFramebuffer;
		}
	}
}

void SwapChain::recreate(const VkExtent2D extent) {
	if (extent.width == swapChainExtent.width && extent.height == swapChainExtent.height)
		return;
	vkDeviceWaitIdle(device);
	cleanup();
	createSwapChain(extent);
	createImageViews();
	createSwapChainFrameBuffers();
}

}// namespace owl::renderer::vulkan::internal
