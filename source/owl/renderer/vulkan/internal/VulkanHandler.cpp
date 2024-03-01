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

constexpr int g_maxFrameInFlight = 2;

}// namespace

VulkanHandler::VulkanHandler() = default;

VulkanHandler::~VulkanHandler() = default;

void VulkanHandler::initVulkan() {
	{
		createCore();
		if (m_state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Core created.")
	}
	{
		createSwapChain();
		if (m_state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Swap Chain created.")
	}
	{
		createDescriptorPool();
		if (m_state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Descriptor pool created.")
	}
	{
		createCommandPool();
		if (m_state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Command Pool Created.")
	}
	{
		createCommandBuffers();
		if (m_state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Command Buffer Created.")
	}
	{
		createSyncObjects();
		if (m_state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Sync objects Created.")
	}
	{
		createDescriptorSetLayout();
		if (m_state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Desciptor set Layout Created.")
	}
	m_state = State::Running;
}

void VulkanHandler::release() {
	auto &core = VulkanCore::get();
	if (core.getInstance() == nullptr)
		return;// nothing can exists without instance.

	for (auto &&[id, pipeLine]: m_pipeLines) {
		if (pipeLine.pipeLine != nullptr)
			vkDestroyPipeline(core.getLogicalDevice(), pipeLine.pipeLine, nullptr);
		if (pipeLine.layout != nullptr)
			vkDestroyPipelineLayout(core.getLogicalDevice(), pipeLine.layout, nullptr);
	}
	m_pipeLines.clear();
	for (size_t i = 0; i < g_maxFrameInFlight; ++i) {
		vkDestroyBuffer(core.getLogicalDevice(), m_uniformBuffers[i], nullptr);
		vkFreeMemory(core.getLogicalDevice(), m_uniformBuffersMemory[i], nullptr);
	}
	if (m_descriptorSetLayout != nullptr) {
		vkDestroyDescriptorSetLayout(core.getLogicalDevice(), m_descriptorSetLayout, nullptr);
		OWL_CORE_TRACE("Vulkan: descriptorSetLayout destroyed.")
	}
	for (size_t i = 0; i < g_maxFrameInFlight; ++i) {
		if (m_renderFinishedSemaphores[i] != nullptr) {
			vkDestroySemaphore(core.getLogicalDevice(), m_renderFinishedSemaphores[i], nullptr);
			OWL_CORE_TRACE("Vulkan: renderFinishedSemaphore destroyed.")
		}
		if (m_imageAvailableSemaphores[i] != nullptr) {
			vkDestroySemaphore(core.getLogicalDevice(), m_imageAvailableSemaphores[i], nullptr);
			OWL_CORE_TRACE("Vulkan: imageAvailableSemaphore destroyed.")
		}
		if (m_inFlightFences[i] != nullptr) {
			vkDestroyFence(core.getLogicalDevice(), m_inFlightFences[i], nullptr);
			OWL_CORE_TRACE("Vulkan: inFlightFence destroyed.")
		}
	}
	if (commandPool != nullptr) {
		vkDestroyCommandPool(core.getLogicalDevice(), commandPool, nullptr);
		OWL_CORE_TRACE("Vulkan: commandPool destroyed.")
	}
	if (m_descriptorPool != nullptr) {
		vkDestroyDescriptorPool(core.getLogicalDevice(), m_descriptorPool, nullptr);
		m_descriptorPool = nullptr;
		OWL_CORE_TRACE("Vulkan: descriptorPool destroyed.")
	}
	m_swapChain.release();
	OWL_CORE_TRACE("Vulkan: swap destroyed.")
	core.release();
	OWL_CORE_TRACE("Vulkan: core destroyed.")
	m_state = State::Uninitialized;
}

static void func(const VkResult iResult) {
	if (iResult != VK_SUCCESS) { OWL_CORE_ERROR("Vulkan Imgui: Error detected: {}", resultString(iResult)) }
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
			.DescriptorPool = m_descriptorPool,
			.Subpass = 0,
			.MinImageCount = 2,
			.ImageCount = 2,
			.MSAASamples = VK_SAMPLE_COUNT_1_BIT,
			.UseDynamicRendering = false,
			.ColorAttachmentFormat = m_swapChain.swapChainImageFormat,
			.Allocator = nullptr,
			.CheckVkResultFn = func,
			.MinAllocationSize = 1024 * 1024};
}

void VulkanHandler::createCore() {
	auto &core = VulkanCore::get();
	core.init({.activeValidation = m_validation});
	if (core.getState() == VulkanCore::State::Error) { m_state = State::ErrorCreatingCore; }
}

void VulkanHandler::createSwapChain() {
	const auto &core = VulkanCore::get();
	m_swapChain.create(core.getLogicalDevice(), core.getPhysicalDevice());
	if (m_swapChain.state != SwapChain::State::Initialized)
		m_state = State::ErrorCreatingSwapChain;
}

void VulkanHandler::createDescriptorPool() {
	const auto &core = VulkanCore::get();
	VkDescriptorPoolSize poolSize{
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = static_cast<uint32_t>(g_maxFrameInFlight)};
	const VkDescriptorPoolCreateInfo poolInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.maxSets = static_cast<uint32_t>(g_maxFrameInFlight),
			.poolSizeCount = 1,
			.pPoolSizes = &poolSize};
	if (const VkResult result = vkCreateDescriptorPool(core.getLogicalDevice(), &poolInfo, nullptr, &m_descriptorPool);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("failed to create descriptor pool ({})", resultString(result))
		m_state = State::ErrorCreatingDescriptorPool;
	}
}

void VulkanHandler::createDescriptorSets(const size_t iSize) {
	const auto &core = VulkanCore::get();
	std::vector<VkDescriptorSetLayout> layouts(g_maxFrameInFlight, m_descriptorSetLayout);
	const VkDescriptorSetAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext = nullptr,
			.descriptorPool = m_descriptorPool,
			.descriptorSetCount = static_cast<uint32_t>(g_maxFrameInFlight),
			.pSetLayouts = layouts.data()};
	m_descriptorSets.resize(g_maxFrameInFlight);
	if (const auto result = vkAllocateDescriptorSets(core.getLogicalDevice(), &allocInfo, m_descriptorSets.data());
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to allocate descriptor sets ({})", resultString(result))
		m_state = State::ErrorCreatingDescriptorSet;
		return;
	}

	for (size_t i = 0; i < g_maxFrameInFlight; i++) {
		VkDescriptorBufferInfo bufferInfo{
				.buffer = m_uniformBuffers[i],
				.offset = 0,
				.range = iSize};
		VkWriteDescriptorSet descriptorWrite{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext = nullptr,
				.dstSet = m_descriptorSets[i],
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

VulkanHandler::PipeLineData VulkanHandler::getPipeline(const int32_t iId) const {
	if (m_state == State::Running || !m_pipeLines.contains(iId))
		return {};
	return m_pipeLines.at(iId);
}

VkCommandBuffer VulkanHandler::getCurrentCommandBuffer() const { return commandBuffers[m_currentFrame]; }

int32_t VulkanHandler::pushPipeline(const std::string &iPipeLineName,
									std::vector<VkPipelineShaderStageCreateInfo> &iShaderStages,
									VkPipelineVertexInputStateCreateInfo iVertexInputInfo) {
	const auto &core = VulkanCore::get();
	PipeLineData pData;
	// PipeLine Layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.setLayoutCount = 1,
			.pSetLayouts = &m_descriptorSetLayout,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr};
	if (const VkResult result =
				vkCreatePipelineLayout(core.getLogicalDevice(), &pipelineLayoutInfo, nullptr, &pData.layout);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: Shader: failed to create pipeline layout {} ({})", iPipeLineName, resultString(result))
		m_state = State::ErrorCreatingPipelineLayout;
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
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
							  VK_COLOR_COMPONENT_A_BIT};
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
			.stageCount = static_cast<uint32_t>(iShaderStages.size()),
			.pStages = iShaderStages.data(),
			.pVertexInputState = &iVertexInputInfo,
			.pInputAssemblyState = &inputAssembly,
			.pTessellationState = nullptr,
			.pViewportState = &viewportState,
			.pRasterizationState = &rasterizer,
			.pMultisampleState = &multisampling,
			.pDepthStencilState = &depthStensil,
			.pColorBlendState = &colorBlending,
			.pDynamicState = &dynamicState,
			.layout = pData.layout,
			.renderPass = m_swapChain.renderPass,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = 0};

	OWL_CORE_TRACE("Vulkan pipeline: vkCreateGraphicsPipelines")
	if (const VkResult result = vkCreateGraphicsPipelines(core.getLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo,
														  nullptr, &pData.pipeLine); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create graphics pipeline for {} ({})", iPipeLineName, resultString(result))
		m_state = State::ErrorCreatingPipeline;
		return -1;
	}

	const auto id = m_pipeLines.empty() ? 0 : static_cast<int32_t>(m_pipeLines.rbegin()->first) + 1;
	m_pipeLines.emplace(id, pData);
	OWL_CORE_TRACE("Vulkan pipeline: {} Loaded.", iPipeLineName)
	return id;
}

void VulkanHandler::popPipeline(const int32_t iId) {
	const auto &core = VulkanCore::get();
	if (!m_pipeLines.contains(iId))
		return;
	if (m_pipeLines[iId].pipeLine != nullptr)
		vkDestroyPipeline(core.getLogicalDevice(), m_pipeLines[iId].pipeLine, nullptr);
	if (m_pipeLines[iId].layout != nullptr)
		vkDestroyPipelineLayout(core.getLogicalDevice(), m_pipeLines[iId].layout, nullptr);
	m_pipeLines.erase(iId);
}

void VulkanHandler::createCommandPool() {
	const auto &core = VulkanCore::get();
	const VkCommandPoolCreateInfo poolInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = core.getGraphQueueFamilyIndex()};

	if (const VkResult result = vkCreateCommandPool(core.getLogicalDevice(), &poolInfo, nullptr, &commandPool);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create command pool ({}).", resultString(result))
		m_state = State::ErrorCreatingCommandPool;
	}
}

void VulkanHandler::createCommandBuffers() {
	const auto &core = VulkanCore::get();
	commandBuffers.resize(g_maxFrameInFlight);
	const VkCommandBufferAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = commandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = static_cast<uint32_t>(commandBuffers.size())};
	if (const VkResult result = vkAllocateCommandBuffers(core.getLogicalDevice(), &allocInfo, commandBuffers.data());
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("failed to allocate command buffers ({}).", resultString(result))
		m_state = State::ErrorCreatingCommandBuffer;
	}
}

void VulkanHandler::createSyncObjects() {
	m_imageAvailableSemaphores.resize(g_maxFrameInFlight);
	m_renderFinishedSemaphores.resize(g_maxFrameInFlight);
	m_inFlightFences.resize(g_maxFrameInFlight);

	constexpr VkSemaphoreCreateInfo semaphoreInfo{
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {}};
	constexpr VkFenceCreateInfo fenceInfo{
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT};

	const auto &core = VulkanCore::get();
	for (size_t i = 0; i < g_maxFrameInFlight; ++i) {
		if (const VkResult result = vkCreateSemaphore(core.getLogicalDevice(), &semaphoreInfo, nullptr,
													  &m_imageAvailableSemaphores[i]); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: failed to create image available semaphore {} ({}).", i, resultString(result))
			m_state = State::ErrorCreatingSyncObjects;
			return;
		}
		if (const VkResult result = vkCreateSemaphore(core.getLogicalDevice(), &semaphoreInfo, nullptr,
													  &m_renderFinishedSemaphores[i]); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: failed to create render finish semaphore {} ({}).", i, resultString(result))
			m_state = State::ErrorCreatingSyncObjects;
			return;
		}
		if (const VkResult result = vkCreateFence(core.getLogicalDevice(), &fenceInfo, nullptr, &m_inFlightFences[i]);
			result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: failed to create synchronization in flight fence {} ({}).", i, resultString(result))
			m_state = State::ErrorCreatingSyncObjects;
			return;
		}
	}
}

void VulkanHandler::setClearColor(const glm::vec4 &iColor) {
	const glm::vec4 lin = math::sRGBToLinear(iColor);
	m_clearColor.color.float32[0] = lin.r;
	m_clearColor.color.float32[1] = lin.g;
	m_clearColor.color.float32[2] = lin.b;
	m_clearColor.color.float32[3] = iColor.a;
}

void VulkanHandler::clear() {
	constexpr VkCommandBufferBeginInfo beginInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = {},
			.pInheritanceInfo = nullptr};
	if (const VkResult result = vkBeginCommandBuffer(getCurrentCommandBuffer(), &beginInfo); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to begin recording command buffer ({}).", resultString(result))
		m_state = State::ErrorBeginCommandBuffer;
		return;
	}

	const VkRenderPassBeginInfo renderPassInfo{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext = nullptr,
			.renderPass = m_swapChain.clearingPass,
			.framebuffer = m_swapChain.swapChainFramebuffers[m_imageIndex],
			.renderArea = {
					.offset = {0, 0},
					.extent = m_swapChain.swapChainExtent},
			.clearValueCount = 1,
			.pClearValues = &m_clearColor};
	vkCmdBeginRenderPass(getCurrentCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdEndRenderPass(getCurrentCommandBuffer());
	if (const VkResult result = vkEndCommandBuffer(getCurrentCommandBuffer()); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to end command buffer ({}).", resultString(result))
		m_state = State::ErrorEndCommandBuffer;
	}
	const VkSubmitInfo submitInfo{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext = nullptr,
			.waitSemaphoreCount = 0,
			.pWaitSemaphores = nullptr,
			.pWaitDstStageMask = nullptr,
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffers[m_currentFrame],
			.signalSemaphoreCount = 0,
			.pSignalSemaphores = nullptr};
	const auto &core = VulkanCore::get();
	vkResetFences(core.getLogicalDevice(), 1, &m_inFlightFences[m_currentFrame]);
	if (const VkResult result = vkQueueSubmit(core.getGraphicQueue(), 1, &submitInfo, m_inFlightFences[m_currentFrame]);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to submit draw command buffer ({}).", resultString(result))
		m_state = State::ErrorSubmitingDrawCommand;
	}
	vkWaitForFences(core.getLogicalDevice(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
}

void VulkanHandler::drawData(const uint32_t iVertexCount, const bool iIndexed) {
	if (m_state != State::Running)
		return;
	if (!inBatch)
		beginBatch();
	if (iIndexed)
		vkCmdDrawIndexed(getCurrentCommandBuffer(), iVertexCount, 1, 0, 0, 0);
	else
		vkCmdDraw(getCurrentCommandBuffer(), iVertexCount, 1, 0, 0);
}

void VulkanHandler::beginFrame() {
	if (m_state != State::Running)
		return;
	const auto &core = VulkanCore::get();
	vkWaitForFences(core.getLogicalDevice(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
	if (const VkResult result = vkAcquireNextImageKHR(core.getLogicalDevice(), m_swapChain.swapChain, UINT64_MAX,
													  m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE,
													  &m_imageIndex); result != VK_SUCCESS) {
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			m_swapChain.recreate(core.getCurrentExtent());
			return;
		}
		if (result != VK_SUBOPTIMAL_KHR) {
			OWL_CORE_ERROR("Vulkan: failed to aquire next image ({}).", resultString(result))
			m_state = State::ErrorAcquiringNextImage;
			return;
		}
		vkResetFences(core.getLogicalDevice(), 1, &m_inFlightFences[m_currentFrame]);
	}
}

void VulkanHandler::endFrame() {
	if (m_state != State::Running)
		return;
	if (inBatch)
		endBatch();
	firstBatch = true;
}

void VulkanHandler::beginBatch() {
	inBatch = true;

	const auto &core = VulkanCore::get();
	vkWaitForFences(core.getLogicalDevice(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
	vkResetFences(core.getLogicalDevice(), 1, &m_inFlightFences[m_currentFrame]);

	if (const VkResult result = vkResetCommandBuffer(getCurrentCommandBuffer(), /*VkCommandBufferResetFlagBits*/ 0);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to reset recording command buffer ({}).", resultString(result))
		m_state = State::ErrorResetCommandBuffer;
		return;
	}
	constexpr VkCommandBufferBeginInfo beginInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = {},
			.pInheritanceInfo = nullptr};
	if (const VkResult result = vkBeginCommandBuffer(getCurrentCommandBuffer(), &beginInfo); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to begin recording command buffer ({}).", resultString(result))
		m_state = State::ErrorBeginCommandBuffer;
		return;
	}
	const VkRenderPassBeginInfo renderPassInfo{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext = nullptr,
			.renderPass = m_swapChain.renderPass,
			.framebuffer = m_swapChain.swapChainFramebuffers[m_imageIndex],
			.renderArea = {
					.offset = {0, 0},
					.extent = m_swapChain.swapChainExtent},
			.clearValueCount = 1,
			.pClearValues = &m_clearColor};
	vkCmdBeginRenderPass(getCurrentCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	const VkViewport viewport{
			.x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(m_swapChain.swapChainExtent.width),
			.height = static_cast<float>(m_swapChain.swapChainExtent.height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f};
	vkCmdSetViewport(getCurrentCommandBuffer(), 0, 1, &viewport);
	const VkRect2D scissor{
			.offset = {0, 0},
			.extent = m_swapChain.swapChainExtent};
	vkCmdSetScissor(getCurrentCommandBuffer(), 0, 1, &scissor);
}

void VulkanHandler::endBatch() {
	vkCmdEndRenderPass(getCurrentCommandBuffer());
	if (const VkResult result = vkEndCommandBuffer(getCurrentCommandBuffer()); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to end command buffer ({}).", resultString(result))
		m_state = State::ErrorEndCommandBuffer;
	}
	const VkSemaphore waitSemaphoresStart[] = {m_imageAvailableSemaphores[m_currentFrame]};
	const VkSemaphore waitSemaphores[] = {m_renderFinishedSemaphores[m_currentFrame]};
	constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	const VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphores[m_currentFrame]};
	const VkSubmitInfo submitInfo{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext = nullptr,
			.waitSemaphoreCount = 1u,
			.pWaitSemaphores = firstBatch ? waitSemaphoresStart : waitSemaphores,
			.pWaitDstStageMask = waitStages,
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffers[m_currentFrame],
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = signalSemaphores};

	const auto &core = VulkanCore::get();
	if (const VkResult result = vkQueueSubmit(core.getGraphicQueue(), 1, &submitInfo, m_inFlightFences[m_currentFrame]);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to submit draw command buffer ({}).", resultString(result))
		m_state = State::ErrorSubmitingDrawCommand;
		return;
	}
	inBatch = false;
	firstBatch = false;
}

void VulkanHandler::swapFrame() {
	if (m_state != State::Running)
		return;
	if (inBatch)
		endFrame();
	const VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphores[m_currentFrame]};
	const VkSwapchainKHR swapChains[] = {m_swapChain.swapChain};
	const VkPresentInfoKHR presentInfo{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.pNext = nullptr,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = signalSemaphores,
			.swapchainCount = 1,
			.pSwapchains = swapChains,
			.pImageIndices = &m_imageIndex,
			.pResults = nullptr};
	const auto &core = VulkanCore::get();
	if (const VkResult result = vkQueuePresentKHR(core.getPresentQueue(), &presentInfo); result != VK_SUCCESS) {
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_resize) {
			m_resize = false;
			m_swapChain.recreate(core.getCurrentExtent());
		} else {
			OWL_CORE_ERROR("Vulkan: failed to present queue ({}).", resultString(result))
			m_state = State::ErrorPresentingQueue;
		}
	}

	m_currentFrame = (m_currentFrame + 1) % g_maxFrameInFlight;
}

void VulkanHandler::bindPipeline(const int32_t iId) {
	if (m_state != State::Running)
		return;
	if (!m_pipeLines.contains(iId)) {
		OWL_CORE_WARN("Vulkan: cannot bind pipeline with id {}", iId)
		return;
	}
	vkCmdBindPipeline(getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeLines[iId].pipeLine);
	vkCmdBindDescriptorSets(getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeLines[iId].layout, 0, 1,
							&m_descriptorSets[m_currentFrame], 0, nullptr);
}

void VulkanHandler::setResize() {
	auto &core = VulkanCore::get();
	core.updateSurfaceInformations();
	m_resize = true;
}

void VulkanHandler::copyBuffer(const VkBuffer &iSrcBuffer, const VkBuffer &iDstBuffer, const VkDeviceSize iSize) const {

	const auto &core = VulkanCore::get();
	const VkCommandBufferAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = commandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1};

	VkCommandBuffer commandBuffer;
	if (const VkResult result = vkAllocateCommandBuffers(core.getLogicalDevice(), &allocInfo, &commandBuffer);
		result != VK_SUCCESS) {
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
	copyRegion.size = iSize;
	vkCmdCopyBuffer(commandBuffer, iSrcBuffer, iDstBuffer, 1, &copyRegion);

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

	if (const VkResult result = vkQueueSubmit(core.getGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		result != VK_SUCCESS) {
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
	if (const auto result =
				vkCreateDescriptorSetLayout(core.getLogicalDevice(), &layoutInfo, nullptr, &m_descriptorSetLayout);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create descriptor set layout ({}).", resultString(result))
		m_state = State::ErrorCreatingDescriptorSetLayout;
	}
}

void VulkanHandler::createUniformBuffers(const size_t iSize) {
	m_uniformBuffers.resize(g_maxFrameInFlight);
	m_uniformBuffersMemory.resize(g_maxFrameInFlight);
	m_uniformBuffersMapped.resize(g_maxFrameInFlight);
	const auto &core = VulkanCore::get();
	for (size_t i = 0; i < g_maxFrameInFlight; i++) {
		createBuffer(iSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
					 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i],
					 m_uniformBuffersMemory[i]);
		if (const auto result = vkMapMemory(core.getLogicalDevice(), m_uniformBuffersMemory[i], 0, iSize, 0,
											&m_uniformBuffersMapped[i]); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: Failed to create uniform buffer {}.", i)
		}
	}
	createDescriptorSets(iSize);
}

void VulkanHandler::createBuffer(const VkDeviceSize iSize, const VkBufferUsageFlags iUsage,
								 const VkMemoryPropertyFlags iProperties,
								 VkBuffer &iBuffer, VkDeviceMemory &iBufferMemory) const {
	const auto &core = VulkanCore::get();
	const VkBufferCreateInfo bufferInfo{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.size = iSize,
			.usage = iUsage,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = nullptr};

	if (const VkResult result = vkCreateBuffer(core.getLogicalDevice(), &bufferInfo, nullptr, &iBuffer);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan buffer: failed to create vertex buffer ({}).", resultString(result))
		return;
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(core.getLogicalDevice(), iBuffer, &memRequirements);

	const VkMemoryAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = nullptr,
			.allocationSize = memRequirements.size,
			.memoryTypeIndex = core.findMemoryTypeIndex(memRequirements.memoryTypeBits, iProperties)};

	if (const VkResult result = vkAllocateMemory(core.getLogicalDevice(), &allocInfo, nullptr, &iBufferMemory);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan vertex buffer: failed to allocate memory buffer ({}).", resultString(result))
		return;
	}

	if (const VkResult result = vkBindBufferMemory(core.getLogicalDevice(), iBuffer, iBufferMemory, 0);
		result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan vertex buffer: failed to bind memory buffer ({}).", resultString(result))
	}
}

void VulkanHandler::setUniformData(const void *iData, const size_t iSize) const {
	memcpy(m_uniformBuffersMapped[m_currentFrame], iData, iSize);
}

}// namespace owl::renderer::vulkan::internal
