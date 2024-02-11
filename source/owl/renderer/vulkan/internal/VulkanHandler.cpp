/**
 * @file VulkanHandler.cpp
 * @author Silmaen
 * @date 30/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "VulkanHandler.h"

#include "../GraphContext.h"
#include "core/Application.h"
#include "math/ColorSpace.h"
#include "renderer/utils/shaderFileUtils.h"
#include "utils.h"

namespace owl::renderer::vulkan::internal {

namespace {

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

}// namespace

VulkanHandler::VulkanHandler() = default;

VulkanHandler::~VulkanHandler() = default;

void VulkanHandler::initVulkan() {
	{
		createCore();
		if (state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Core created.")
	}
	{
		createSwapChain();
		if (state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Swap Chain created.")
	}
	{
		createDescriptorPool();
		if (state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Descriptor pool created.")
	}
	{
		createCommandPool();
		if (state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Command Pool Created.")
	}
	{
		createCommandBuffers();
		if (state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Command Buffer Created.")
	}
	{
		createSyncObjects();
		if (state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Sync objects Created.")
	}
	{
		createDescriptorSetLayout();
		if (state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Desciptor set Layout Created.")
	}
	state = State::Running;
}

void VulkanHandler::release() {
	auto &core = VulkanCore::get();
	if (core.getInstance() == nullptr) return;// nothing can exists without instance.

	for (auto &&[id, pipeLine]: pipeLines) {
		if (pipeLine.pipeLine != nullptr)
			vkDestroyPipeline(core.getLogicalDevice(), pipeLine.pipeLine, nullptr);
		if (pipeLine.layout != nullptr)
			vkDestroyPipelineLayout(core.getLogicalDevice(), pipeLine.layout, nullptr);
	}
	pipeLines.clear();
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		vkDestroyBuffer(core.getLogicalDevice(), uniformBuffers[i], nullptr);
		vkFreeMemory(core.getLogicalDevice(), uniformBuffersMemory[i], nullptr);
	}
	if (descriptorSetLayout != nullptr) {
		vkDestroyDescriptorSetLayout(core.getLogicalDevice(), descriptorSetLayout, nullptr);
		OWL_CORE_TRACE("Vulkan: descriptorSetLayout destroyed.")
	}
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		if (renderFinishedSemaphores[i] != nullptr) {
			vkDestroySemaphore(core.getLogicalDevice(), renderFinishedSemaphores[i], nullptr);
			OWL_CORE_TRACE("Vulkan: renderFinishedSemaphore destroyed.")
		}
		if (imageAvailableSemaphores[i] != nullptr) {
			vkDestroySemaphore(core.getLogicalDevice(), imageAvailableSemaphores[i], nullptr);
			OWL_CORE_TRACE("Vulkan: imageAvailableSemaphore destroyed.")
		}
		if (inFlightFences[i] != nullptr) {
			vkDestroyFence(core.getLogicalDevice(), inFlightFences[i], nullptr);
			OWL_CORE_TRACE("Vulkan: inFlightFence destroyed.")
		}
	}
	if (commandPool != nullptr) {
		vkDestroyCommandPool(core.getLogicalDevice(), commandPool, nullptr);
		OWL_CORE_TRACE("Vulkan: commandPool destroyed.")
	}
	if (descriptorPool != nullptr) {
		vkDestroyDescriptorPool(core.getLogicalDevice(), descriptorPool, nullptr);
		descriptorPool = nullptr;
		OWL_CORE_TRACE("Vulkan: descriptorPool destroyed.")
	}
	swapChain.release();
	OWL_CORE_TRACE("Vulkan: swap destroyed.")
	core.release();
	OWL_CORE_TRACE("Vulkan: core destroyed.")
	state = State::Uninitialized;
}

static void func(VkResult result) {
	if (result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan Imgui: Error detected: {}", resultString(result))
	}
}

ImGui_ImplVulkan_InitInfo VulkanHandler::toImGuiInfo() const {
	const auto &core = VulkanCore::get();
	return {
			.Instance = core.getInstance(),
			.PhysicalDevice = core.getPhysicalDevice(),
			.Device = core.getLogicalDevice(),
			.QueueFamily = core.getGraphQueueFamilyIndex(),
			.Queue = core.getGraphicQueue(),
			.PipelineCache = {},
			.DescriptorPool = descriptorPool,
			.Subpass = 0,
			.MinImageCount = 2,
			.ImageCount = 2,
			.MSAASamples = VK_SAMPLE_COUNT_1_BIT,
			.UseDynamicRendering = false,
			.ColorAttachmentFormat = swapChain.swapChainImageFormat,
			.Allocator = nullptr,
			.CheckVkResultFn = func,
			.MinAllocationSize = 1024 * 1024};
}

void VulkanHandler::createCore() {
	auto &core = VulkanCore::get();
	core.init({.activeValidation = validation});
	if (core.getState() == VulkanCore::State::Error) {
		state = State::ErrorCreatingCore;
	}
}

void VulkanHandler::createSwapChain() {
	const auto &core = VulkanCore::get();
	swapChain.create(core.getLogicalDevice(), core.getPhysicalDevice());
	if (swapChain.state != SwapChain::State::Initialized)
		state = State::ErrorCreatingSwapChain;
}

void VulkanHandler::createDescriptorPool() {
	const auto &core = VulkanCore::get();
	VkDescriptorPoolSize poolSize{
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)};
	const VkDescriptorPoolCreateInfo poolInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
			.poolSizeCount = 1,
			.pPoolSizes = &poolSize};
	if (const VkResult result = vkCreateDescriptorPool(core.getLogicalDevice(), &poolInfo, nullptr, &descriptorPool); result != VK_SUCCESS) {
		OWL_CORE_ERROR("failed to create descriptor pool ({})", resultString(result))
		state = State::ErrorCreatingDescriptorPool;
	}
}

void VulkanHandler::createDescriptorSets(const size_t size) {
	const auto &core = VulkanCore::get();
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
	const VkDescriptorSetAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext = nullptr,
			.descriptorPool = descriptorPool,
			.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
			.pSetLayouts = layouts.data()};
	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (const auto result = vkAllocateDescriptorSets(core.getLogicalDevice(), &allocInfo, descriptorSets.data()); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to allocate descriptor sets ({})", resultString(result))
		state = State::ErrorCreatingDescriptorSet;
		return;
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{
				.buffer = uniformBuffers[i],
				.offset = 0,
				.range = size};
		VkWriteDescriptorSet descriptorWrite{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext = nullptr,
				.dstSet = descriptorSets[i],
				.dstBinding = 0,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.pImageInfo = nullptr,
				.pBufferInfo = &bufferInfo,
				.pTexelBufferView = nullptr};
		vkUpdateDescriptorSets(core.getLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
	}
}

VulkanHandler::PipeLineData VulkanHandler::getPipeline(const int32_t id) const {
	if (state == State::Running || !pipeLines.contains(id))
		return {};
	return pipeLines.at(id);
}

VkCommandBuffer VulkanHandler::getCurrentCommandBuffer() const {
	return commandBuffers[currentFrame];
}

int32_t VulkanHandler::pushPipeline(const std::string &pipeLineName, std::vector<VkPipelineShaderStageCreateInfo> &shaderStages, VkPipelineVertexInputStateCreateInfo vertexInputInfo) {
	const auto &core = VulkanCore::get();
	PipeLineData pData;
	// PipeLine Layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.setLayoutCount = 1,
			.pSetLayouts = &descriptorSetLayout,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr};
	if (const VkResult result = vkCreatePipelineLayout(core.getLogicalDevice(), &pipelineLayoutInfo, nullptr, &pData.layout); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: Shader: failed to create pipeline layout {} ({})", pipeLineName, resultString(result))
		state = State::ErrorCreatingPipelineLayout;
		return -1;
	}

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE};
	VkPipelineViewportStateCreateInfo viewportState{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.viewportCount = 1,
			.pViewports = nullptr,
			.scissorCount = 1,
			.pScissors = nullptr};
	VkPipelineRasterizationStateCreateInfo rasterizer{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.depthClampEnable = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode = VK_POLYGON_MODE_FILL,// VK_POLYGON_MODE_LINE
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.frontFace = VK_FRONT_FACE_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0.0f,
			.depthBiasClamp = 0.0,
			.depthBiasSlopeFactor = 0.0f,
			.lineWidth = 1.0f};
	VkPipelineMultisampleStateCreateInfo multisampling{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable = VK_FALSE,
			.minSampleShading = 0.0f,
			.pSampleMask = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable = VK_FALSE};
	VkPipelineColorBlendAttachmentState colorBlendAttachment{
			.blendEnable = VK_TRUE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.alphaBlendOp = {},
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};
	VkPipelineColorBlendStateCreateInfo colorBlending{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = &colorBlendAttachment,
			.blendConstants = {0.f, 0.f, 0.f, 0.f}};
	VkDynamicState dynamicStates[] = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR};
	VkPipelineDynamicStateCreateInfo dynamicState{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.dynamicStateCount = 2,
			.pDynamicStates = dynamicStates};
	VkPipelineDepthStencilStateCreateInfo depthStensil{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.depthTestEnable = VK_TRUE,
			.depthWriteEnable = VK_TRUE,
			.depthCompareOp = VK_COMPARE_OP_LESS,
			.depthBoundsTestEnable = VK_FALSE,
			.stencilTestEnable = VK_FALSE,
			.front = {},
			.back = {},
			.minDepthBounds = 0.0f,
			.maxDepthBounds = 1.0f};
	VkGraphicsPipelineCreateInfo pipelineInfo{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.stageCount = static_cast<uint32_t>(shaderStages.size()),
			.pStages = shaderStages.data(),
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = &inputAssembly,
			.pTessellationState = nullptr,
			.pViewportState = &viewportState,
			.pRasterizationState = &rasterizer,
			.pMultisampleState = &multisampling,
			.pDepthStencilState = &depthStensil,
			.pColorBlendState = &colorBlending,
			.pDynamicState = &dynamicState,
			.layout = pData.layout,
			.renderPass = swapChain.renderPass,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = 0};

	OWL_CORE_TRACE("Vulkan pipeline: vkCreateGraphicsPipelines")
	if (const VkResult result = vkCreateGraphicsPipelines(core.getLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pData.pipeLine); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create graphics pipeline for {} ({})", pipeLineName, resultString(result))
		state = State::ErrorCreatingPipeline;
		return -1;
	}

	const auto id = pipeLines.empty() ? 0 : static_cast<int32_t>(pipeLines.rbegin()->first) + 1;
	pipeLines.emplace(id, pData);
	OWL_CORE_TRACE("Vulkan pipeline: {} Loaded.", pipeLineName)
	return id;
}

void VulkanHandler::popPipeline(const int32_t id) {
	const auto &core = VulkanCore::get();
	if (!pipeLines.contains(id))
		return;
	if (pipeLines[id].pipeLine != nullptr)
		vkDestroyPipeline(core.getLogicalDevice(), pipeLines[id].pipeLine, nullptr);
	if (pipeLines[id].layout != nullptr)
		vkDestroyPipelineLayout(core.getLogicalDevice(), pipeLines[id].layout, nullptr);
	pipeLines.erase(id);
}

void VulkanHandler::createCommandPool() {
	const auto &core = VulkanCore::get();
	const VkCommandPoolCreateInfo poolInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = core.getGraphQueueFamilyIndex()};

	if (const VkResult result = vkCreateCommandPool(core.getLogicalDevice(), &poolInfo, nullptr, &commandPool); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create command pool ({}).", resultString(result))
		state = State::ErrorCreatingCommandPool;
	}
}

void VulkanHandler::createCommandBuffers() {
	const auto &core = VulkanCore::get();
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	const VkCommandBufferAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = commandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = static_cast<uint32_t>(commandBuffers.size())};
	if (const VkResult result = vkAllocateCommandBuffers(core.getLogicalDevice(), &allocInfo, commandBuffers.data()); result != VK_SUCCESS) {
		OWL_CORE_ERROR("failed to allocate command buffers ({}).", resultString(result))
		state = State::ErrorCreatingCommandBuffer;
	}
}

void VulkanHandler::createSyncObjects() {
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	constexpr VkSemaphoreCreateInfo semaphoreInfo{
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {}};
	constexpr VkFenceCreateInfo fenceInfo{
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT};

	const auto &core = VulkanCore::get();
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		if (const VkResult result = vkCreateSemaphore(core.getLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: failed to create image available semaphore {} ({}).", i, resultString(result))
			state = State::ErrorCreatingSyncObjects;
			return;
		}
		if (const VkResult result = vkCreateSemaphore(core.getLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: failed to create render finish semaphore {} ({}).", i, resultString(result))
			state = State::ErrorCreatingSyncObjects;
			return;
		}
		if (const VkResult result = vkCreateFence(core.getLogicalDevice(), &fenceInfo, nullptr, &inFlightFences[i]); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: failed to create synchronization in flight fence {} ({}).", i, resultString(result))
			state = State::ErrorCreatingSyncObjects;
			return;
		}
	}
}

void VulkanHandler::setClearColor(const glm::vec4 &color) {
	const glm::vec4 lin = math::sRGBToLinear(color);
	clearColor.color.float32[0] = lin.r;
	clearColor.color.float32[1] = lin.g;
	clearColor.color.float32[2] = lin.b;
	clearColor.color.float32[3] = color.a;
}

void VulkanHandler::clear() {
	constexpr VkCommandBufferBeginInfo beginInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = {},
			.pInheritanceInfo = nullptr};
	if (const VkResult result = vkBeginCommandBuffer(getCurrentCommandBuffer(), &beginInfo); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to begin recording command buffer ({}).", resultString(result))
		state = State::ErrorBeginCommandBuffer;
		return;
	}

	const VkRenderPassBeginInfo renderPassInfo{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext = nullptr,
			.renderPass = swapChain.clearingPass,
			.framebuffer = swapChain.swapChainFramebuffers[imageIndex],
			.renderArea = {
					.offset = {0, 0},
					.extent = swapChain.swapChainExtent},
			.clearValueCount = 1,
			.pClearValues = &clearColor};
	vkCmdBeginRenderPass(getCurrentCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdEndRenderPass(getCurrentCommandBuffer());
	if (const VkResult result = vkEndCommandBuffer(getCurrentCommandBuffer()); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to end command buffer ({}).", resultString(result))
		state = State::ErrorEndCommandBuffer;
	}
	const VkSubmitInfo submitInfo{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext = nullptr,
			.waitSemaphoreCount = 0,
			.pWaitSemaphores = nullptr,
			.pWaitDstStageMask = nullptr,
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffers[currentFrame],
			.signalSemaphoreCount = 0,
			.pSignalSemaphores = nullptr};
	const auto &core = VulkanCore::get();
	vkResetFences(core.getLogicalDevice(), 1, &inFlightFences[currentFrame]);
	if (const VkResult result = vkQueueSubmit(core.getGraphicQueue(), 1, &submitInfo, inFlightFences[currentFrame]); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to submit draw command buffer ({}).", resultString(result))
		state = State::ErrorSubmitingDrawCommand;
	}
	vkWaitForFences(core.getLogicalDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
}

void VulkanHandler::drawData(uint32_t vertexCount, bool indexed) {
	if (state != State::Running)
		return;
	if (!inBatch)
		beginBatch();
	if (indexed)
		vkCmdDrawIndexed(getCurrentCommandBuffer(), vertexCount, 1, 0, 0, 0);
	else
		vkCmdDraw(getCurrentCommandBuffer(), vertexCount, 1, 0, 0);
}

void VulkanHandler::beginFrame() {
	if (state != State::Running)
		return;
	const auto &core = VulkanCore::get();
	vkWaitForFences(core.getLogicalDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	if (const VkResult result = vkAcquireNextImageKHR(core.getLogicalDevice(), swapChain.swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex); result != VK_SUCCESS) {
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			swapChain.recreate(core.getCurrentExtent());
			return;
		}
		if (result != VK_SUBOPTIMAL_KHR) {
			OWL_CORE_ERROR("Vulkan: failed to aquire next image ({}).", resultString(result))
			state = State::ErrorAcquiringNextImage;
			return;
		}
		vkResetFences(core.getLogicalDevice(), 1, &inFlightFences[currentFrame]);
	}
}

void VulkanHandler::endFrame() {
	if (state != State::Running)
		return;
	if (inBatch)
		endBatch();
	firstBatch = true;
}

void VulkanHandler::beginBatch() {
	inBatch = true;

	const auto &core = VulkanCore::get();
	vkWaitForFences(core.getLogicalDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	vkResetFences(core.getLogicalDevice(), 1, &inFlightFences[currentFrame]);

	if (const VkResult result = vkResetCommandBuffer(getCurrentCommandBuffer(), /*VkCommandBufferResetFlagBits*/ 0); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to reset recording command buffer ({}).", resultString(result))
		state = State::ErrorResetCommandBuffer;
		return;
	}
	constexpr VkCommandBufferBeginInfo beginInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = {},
			.pInheritanceInfo = nullptr};
	if (const VkResult result = vkBeginCommandBuffer(getCurrentCommandBuffer(), &beginInfo); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to begin recording command buffer ({}).", resultString(result))
		state = State::ErrorBeginCommandBuffer;
		return;
	}
	const VkRenderPassBeginInfo renderPassInfo{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext = nullptr,
			.renderPass = swapChain.renderPass,
			.framebuffer = swapChain.swapChainFramebuffers[imageIndex],
			.renderArea = {
					.offset = {0, 0},
					.extent = swapChain.swapChainExtent},
			.clearValueCount = 1,
			.pClearValues = &clearColor};
	vkCmdBeginRenderPass(getCurrentCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	const VkViewport viewport{
			.x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(swapChain.swapChainExtent.width),
			.height = static_cast<float>(swapChain.swapChainExtent.height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f};
	vkCmdSetViewport(getCurrentCommandBuffer(), 0, 1, &viewport);
	const VkRect2D scissor{
			.offset = {0, 0},
			.extent = swapChain.swapChainExtent};
	vkCmdSetScissor(getCurrentCommandBuffer(), 0, 1, &scissor);
}

void VulkanHandler::endBatch() {
	vkCmdEndRenderPass(getCurrentCommandBuffer());
	if (const VkResult result = vkEndCommandBuffer(getCurrentCommandBuffer()); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to end command buffer ({}).", resultString(result))
		state = State::ErrorEndCommandBuffer;
	}
	const VkSemaphore waitSemaphoresStart[] = {imageAvailableSemaphores[currentFrame]};
	const VkSemaphore waitSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	const VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	const VkSubmitInfo submitInfo{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext = nullptr,
			.waitSemaphoreCount = 1u,
			.pWaitSemaphores = firstBatch ? waitSemaphoresStart : waitSemaphores,
			.pWaitDstStageMask = waitStages,
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffers[currentFrame],
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = signalSemaphores};

	const auto &core = VulkanCore::get();
	if (const VkResult result = vkQueueSubmit(core.getGraphicQueue(), 1, &submitInfo, inFlightFences[currentFrame]); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to submit draw command buffer ({}).", resultString(result))
		state = State::ErrorSubmitingDrawCommand;
		return;
	}
	inBatch = false;
	firstBatch = false;
}

void VulkanHandler::swapFrame() {
	if (state != State::Running)
		return;
	if (inBatch)
		endFrame();
	const VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	const VkSwapchainKHR swapChains[] = {swapChain.swapChain};
	const VkPresentInfoKHR presentInfo{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.pNext = nullptr,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = signalSemaphores,
			.swapchainCount = 1,
			.pSwapchains = swapChains,
			.pImageIndices = &imageIndex,
			.pResults = nullptr};
	const auto &core = VulkanCore::get();
	if (const VkResult result = vkQueuePresentKHR(core.getPresentQueue(), &presentInfo); result != VK_SUCCESS) {
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || resize) {
			resize = false;
			swapChain.recreate(core.getCurrentExtent());
		} else {
			OWL_CORE_ERROR("Vulkan: failed to present queue ({}).", resultString(result))
			state = State::ErrorPresentingQueue;
		}
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanHandler::bindPipeline(const int32_t id) {
	if (state != State::Running)
		return;
	if (!pipeLines.contains(id)) {
		OWL_CORE_WARN("Vulkan: cannot bind pipeline with id {}", id)
		return;
	}
	vkCmdBindPipeline(getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLines[id].pipeLine);
	vkCmdBindDescriptorSets(getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLines[id].layout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);
}

void VulkanHandler::setResize() {
	auto &core = VulkanCore::get();
	core.updateSurfaceInformations();
	resize = true;
}

void VulkanHandler::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const {

	const auto &core = VulkanCore::get();
	const VkCommandBufferAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = commandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1};

	VkCommandBuffer commandBuffer;
	if (const VkResult result = vkAllocateCommandBuffers(core.getLogicalDevice(), &allocInfo, &commandBuffer); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create command buffer for buffer copy.")
		return;
	}

	constexpr VkCommandBufferBeginInfo beginInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			.pInheritanceInfo = nullptr};
	if (const VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to begin command buffer for buffer copy.")
		return;
	}

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	if (const VkResult result = vkEndCommandBuffer(commandBuffer); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to end command buffer for buffer copy.")
		return;
	}

	const VkSubmitInfo submitInfo{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext = nullptr,
			.waitSemaphoreCount = 0,
			.pWaitSemaphores = nullptr,
			.pWaitDstStageMask = nullptr,
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffer,
			.signalSemaphoreCount = 0,
			.pSignalSemaphores = nullptr};

	if (const VkResult result = vkQueueSubmit(core.getGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to submit to queue for buffer copy.")
		return;
	}
	if (const VkResult result = vkQueueWaitIdle(core.getGraphicQueue()); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to wait for idle queue for buffer copy.")
		return;
	}

	vkFreeCommandBuffers(core.getLogicalDevice(), commandPool, 1, &commandBuffer);
}

void VulkanHandler::createDescriptorSetLayout() {
	const auto &core = VulkanCore::get();
	VkDescriptorSetLayoutBinding uboLayoutBinding{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
			.pImmutableSamplers = nullptr};
	const VkDescriptorSetLayoutCreateInfo layoutInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.bindingCount = 1,
			.pBindings = &uboLayoutBinding};
	if (const auto result = vkCreateDescriptorSetLayout(core.getLogicalDevice(), &layoutInfo, nullptr, &descriptorSetLayout); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create descriptor set layout ({}).", resultString(result))
		state = State::ErrorCreatingDescriptorSetLayout;
	}
}

void VulkanHandler::createUniformBuffers(const size_t size) {
	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
	const auto &core = VulkanCore::get();
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
		if (const auto result = vkMapMemory(core.getLogicalDevice(), uniformBuffersMemory[i], 0, size, 0, &uniformBuffersMapped[i]); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: Failed to create uniform buffer {}.", i)
		}
	}
	createDescriptorSets(size);
}

void VulkanHandler::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory) const {
	const auto &core = VulkanCore::get();
	const VkBufferCreateInfo bufferInfo{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.size = size,
			.usage = usage,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = nullptr};

	if (const VkResult result = vkCreateBuffer(core.getLogicalDevice(), &bufferInfo, nullptr, &buffer); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan buffer: failed to create vertex buffer ({}).", resultString(result))
		return;
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(core.getLogicalDevice(), buffer, &memRequirements);

	const VkMemoryAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = nullptr,
			.allocationSize = memRequirements.size,
			.memoryTypeIndex = core.findMemoryTypeIndex(memRequirements.memoryTypeBits, properties)};

	if (const VkResult result = vkAllocateMemory(core.getLogicalDevice(), &allocInfo, nullptr, &bufferMemory); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan vertex buffer: failed to allocate memory buffer ({}).", resultString(result))
		return;
	}

	if (const VkResult result = vkBindBufferMemory(core.getLogicalDevice(), buffer, bufferMemory, 0); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan vertex buffer: failed to bind memory buffer ({}).", resultString(result))
	}
}

void VulkanHandler::setUniformData(const void *data, size_t size) const {
	memcpy(uniformBuffersMapped[currentFrame], data, size);
}

}// namespace owl::renderer::vulkan::internal
