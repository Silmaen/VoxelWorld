/**
 * @file VulkanHandler.cpp
 * @author Silmaen
 * @date 30/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "VulkanHandler.h"

#include "../DrawData.h"
#include "../GraphContext.h"
#include "core/Application.h"
#include "renderer/utils/shaderFileUtils.h"
#include "utils.h"

namespace owl::renderer::vulkan::internal {

namespace {

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;

VkBool32 debugUtilsMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
								   VkDebugUtilsMessageTypeFlagsEXT,
								   const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
								   void *) {

	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
		OWL_CORE_TRACE("Vulkan: [{}][{}]: {}", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage)
	} else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
		OWL_CORE_INFO("Vulkan: [{}][{}]: {}", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage)
	} else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		OWL_CORE_WARN("Vulkan: [{}][{}]: {}", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage)
	} else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		OWL_CORE_ERROR("Vulkan: [{}][{}]: {}", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage)
	}
	// The return value of this callback controls whether the Vulkan call that caused the validation message will be aborted or not
	// We return VK_FALSE as we DON'T want Vulkan calls that cause a validation message to abort
	// If you instead want to have calls abort, pass in VK_TRUE and the function will return VK_ERROR_VALIDATION_FAILED_EXT
	return false;
}

// for debug
//DrawData data;

}// namespace

VulkanHandler::VulkanHandler() = default;

VulkanHandler::~VulkanHandler() = default;

void VulkanHandler::initVulkan() {
	createInstance();
	if (state == State::ErrorCreatingInstance)
		return;
	OWL_CORE_TRACE("Vulkan: Instance created.")
	if (validation) {
		setupDebugging();
		if (state == State::ErrorSetupDebugging) return;
		OWL_CORE_TRACE("Vulkan: Debug support initiated.")
	}
	{
		createSurface();
		if (state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Surface created.")
	}
	{
		createPhysicalDevice();
		if (state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Physical Device created.")
	}
	{
		createLogicalDevice();
		if (state != State::Uninitialized)
			return;
		OWL_CORE_TRACE("Vulkan: Logical Device created.")
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
	if (instance == nullptr) return;// nothing can exists without instance.


	for (auto &&[id, pipeLine]: pipeLines) {
		if (pipeLine.pipeLine != nullptr)
			vkDestroyPipeline(logicalDevice, pipeLine.pipeLine, nullptr);
		if (pipeLine.layout != nullptr)
			vkDestroyPipelineLayout(logicalDevice, pipeLine.layout, nullptr);
	}
	pipeLines.clear();

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		vkDestroyBuffer(logicalDevice, uniformBuffers[i], nullptr);
		vkFreeMemory(logicalDevice, uniformBuffersMemory[i], nullptr);
	}

	if (descriptorSetLayout != nullptr) {
		vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
		OWL_CORE_TRACE("Vulkan: descriptorSetLayout destroyed.")
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		if (renderFinishedSemaphores[i] != nullptr) {
			vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
			OWL_CORE_TRACE("Vulkan: renderFinishedSemaphore destroyed.")
		}
		if (imageAvailableSemaphores[i] != nullptr) {
			vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
			OWL_CORE_TRACE("Vulkan: imageAvailableSemaphore destroyed.")
		}
		if (inFlightFences[i] != nullptr) {
			vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
			OWL_CORE_TRACE("Vulkan: inFlightFence destroyed.")
		}
	}
	if (commandPool != nullptr) {
		vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
		OWL_CORE_TRACE("Vulkan: commandPool destroyed.")
	}
	swapChain.release();
	OWL_CORE_TRACE("Vulkan: swap destroyed.")
	if (logicalDevice != nullptr) {
		vkDestroyDevice(logicalDevice, nullptr);
		logicalDevice = nullptr;
		OWL_CORE_TRACE("Vulkan: logicalDevice destroyed.")
	}
	{
		const auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());
		gc->destroySurface(instance);
		OWL_CORE_TRACE("Vulkan: Surface destroyed.")
	}
	if (debugUtilsMessenger != nullptr) {
		vkDestroyDebugUtilsMessengerEXT(instance, debugUtilsMessenger, nullptr);
		debugUtilsMessenger = nullptr;
		OWL_CORE_TRACE("Vulkan: debugUtilsMessenger destroyed.")
	}
	vkDestroyInstance(instance, nullptr);
	instance = nullptr;
	OWL_CORE_TRACE("Vulkan: instance destroyed.")
	state = State::Uninitialized;
}

static void func(VkResult) {}

ImGui_ImplVulkan_InitInfo VulkanHandler::toImGuiInfo() const {
	return {
			.Instance = instance,
			.PhysicalDevice = physicalDevice.device,
			.Device = logicalDevice,
			.QueueFamily = physicalDevice.queues.graphicsIndex,
			.Queue = physicalDevice.queues.graphics,
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

void VulkanHandler::createInstance() {

	uint32_t instanceVersion;
	vkEnumerateInstanceVersion(&instanceVersion);
	version = static_cast<int>(instanceVersion & 0xFFFFF000);//Remove the patch version.
	OWL_CORE_INFO("Vulkan: Found API Version: {}.{}.", VK_API_VERSION_MAJOR(version), VK_API_VERSION_MINOR(version))

	// App info
	constexpr VkApplicationInfo appInfo = {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = "Owl",
			.applicationVersion = {},
			.pEngineName = "OwlEngine",
			.engineVersion = {},
			.apiVersion = VK_API_VERSION_1_3};

	// Supported Instance extensions & Layer
	{
		// Get extensions supported by the instance and store for later use
		uint32_t extCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
		if (extCount > 0) {
			std::vector<VkExtensionProperties> extensions(extCount);
			if (const VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()); result == VK_SUCCESS) {
				for (const VkExtensionProperties &extension: extensions) {
					supportedInstanceExtensions.emplace_back(extension.extensionName);
					OWL_CORE_TRACE("Vulkan: Supported instance extension: {}", extension.extensionName)
				}
			} else {
				OWL_CORE_WARN("Vulkan: unable to enumerate extensions ({}).", resultString(result))
			}
		}
	}

	// extension for the instance creation.
	std::vector<const char *> instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME};
	// Include Extension from glfw.
	{
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::set<const char *> uniqueExtensions(instanceExtensions.begin(), instanceExtensions.end());
		uniqueExtensions.insert(glfwExtensions, glfwExtensions + glfwExtensionCount);
		instanceExtensions.assign(uniqueExtensions.begin(), uniqueExtensions.end());
	}
	if (validation && std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == supportedInstanceExtensions.end()) {
		OWL_CORE_WARN("Vulkan: request extension Debug Utils but is not supported, deactivating.")
		validation = false;
	}

	// create Info
	VkInstanceCreateInfo instanceCI{};
	instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCI.pApplicationInfo = &appInfo;
	// potentially include Debug Messager
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessagerCI{};
	if (validation) {
		debugUtilsMessagerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugUtilsMessagerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugUtilsMessagerCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		debugUtilsMessagerCI.pfnUserCallback = debugUtilsMessageCallback;
		debugUtilsMessagerCI.pNext = instanceCI.pNext;
		instanceCI.pNext = &debugUtilsMessagerCI;
		instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	// Give the list of required extention to the Instance Create Info
	instanceCI.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	instanceCI.ppEnabledExtensionNames = instanceExtensions.data();

	if (validation) {
		const auto validationLayerName = "VK_LAYER_KHRONOS_validation";
		// Check if this layer is available at instance level
		uint32_t instanceLayerCount;
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
		std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
		OWL_CORE_TRACE("Vulkan: Found {} instance layer.", instanceLayerCount)
		validationPresent = false;
		for (const VkLayerProperties &layer: instanceLayerProperties) {
			OWL_CORE_TRACE("Vulkan: Instance layer: {}", layer.layerName)
			if (strcmp(layer.layerName, validationLayerName) == 0) {
				validationPresent = true;
				break;
			}
		}
		if (validationPresent) {
			instanceCI.ppEnabledLayerNames = &validationLayerName;
			instanceCI.enabledLayerCount = 1;
		} else {
			OWL_CORE_WARN("Vulkan: Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled")
		}
	}

	VkResult result = vkCreateInstance(&instanceCI, nullptr, &instance);
	bool second = false;
	if (result == VK_ERROR_LAYER_NOT_PRESENT) {
		OWL_CORE_ERROR("Vulkan: Unable to create instance because a layer can't be loaded, Retry without layers.")
		second = true;
		instanceCI.ppEnabledLayerNames = nullptr;
		instanceCI.enabledLayerCount = 0;
		result = vkCreateInstance(&instanceCI, nullptr, &instance);
	}
	if (result != VK_SUCCESS) {
		if (second)
			OWL_CORE_ERROR("Vulkan: Definitively unable to create instance ({}).", resultString(result))
		else
			OWL_CORE_ERROR("Vulkan: Unable to create instance ({}).", resultString(result))
		state = State::ErrorCreatingInstance;
	}
}

void VulkanHandler::setupDebugging() {
	OWL_DIAG_PUSH
	OWL_DIAG_DISABLE_CLANG17("-Wcast-function-type-strict")
	vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
	vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
	OWL_DIAG_POP
	constexpr VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessagerCI{
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.pNext = nullptr,
			.flags = {},
			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
			.pfnUserCallback = debugUtilsMessageCallback,
			.pUserData = nullptr};
	if (const VkResult result = vkCreateDebugUtilsMessengerEXT(instance, &debugUtilsMessagerCI, nullptr, &debugUtilsMessenger); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: Error while setup debugging ({})", resultString(result))
		state = State::ErrorSetupDebugging;
	}
}

void VulkanHandler::createSurface() {
	const auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());
	if (const VkResult result = gc->createSurface(instance); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create window surface ({})", resultString(result))
		state = State::ErrorCreatingWindowSurface;
	}
}

void VulkanHandler::createPhysicalDevice() {
	uint32_t deviceCount = 0;
	{
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			OWL_CORE_ERROR("Vulkan: Cannot find any GPUs with vulkan support.")
			state = State::ErrorNoGpuFound;
			return;
		}
	}
	bool found = false;
	{
		physicalDevice.score = 0;
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		for (const auto &device: devices) {
			PhysicalDevice dev;
			dev.create(device);
			OWL_CORE_TRACE("Vulkan: found device: {} @ api {}", dev.getName(), dev.getVersonStr())
			for (const auto &layer: dev.layers) {
				OWL_CORE_TRACE("        layer       : {} {}", layer.layerName, layer.description)
			}
			if (dev.score > physicalDevice.score) {
				physicalDevice = std::move(dev);
				found = true;
			}
		}
	}
	if (!found) {
		OWL_CORE_ERROR("Vulkan: Found no graphics card with enough capabilities.")
		state = State::ErrorNoGpuFound;
		return;
	}
	OWL_CORE_INFO("Vulkan: Using GPU: {}.", physicalDevice.getName())
}

void VulkanHandler::createLogicalDevice() {
	std::vector<VkDeviceQueueCreateInfo> deviceQueuesCI;
	float queuePriority = 1.0f;
	for (const uint32_t queue: physicalDevice.queues.getUniqueIndices()) {
		VkDeviceQueueCreateInfo deviceQueueCI{
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.pNext = nullptr,
				.flags = {},
				.queueFamilyIndex = queue,
				.queueCount = 1,
				.pQueuePriorities = &queuePriority};
		deviceQueuesCI.push_back(deviceQueueCI);
	}
	const auto validationLayerName = "VK_LAYER_KHRONOS_validation";
	const VkDeviceCreateInfo deviceCI{
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.queueCreateInfoCount = static_cast<uint32_t>(deviceQueuesCI.size()),
			.pQueueCreateInfos = deviceQueuesCI.data(),
			.enabledLayerCount = validationPresent ? 1u : 0u,
			.ppEnabledLayerNames = &validationLayerName,
			.enabledExtensionCount = static_cast<uint32_t>(internal::deviceExtensions.size()),
			.ppEnabledExtensionNames = internal::deviceExtensions.data(),
			.pEnabledFeatures = &physicalDevice.enabledFeatures};
	if (const VkResult result = vkCreateDevice(physicalDevice.device, &deviceCI, nullptr, &logicalDevice); result != VK_SUCCESS) {
		state = State::ErrorCreatingLogicalDevice;
		OWL_CORE_ERROR("Vulkan: Error while creating the logical device ({})", resultString(result))
		return;
	}

	physicalDevice.queues.defineQueues(logicalDevice);
}

void VulkanHandler::createSwapChain() {
	swapChain.create(logicalDevice, physicalDevice);
	if (swapChain.state != SwapChain::State::Initialized)
		state = State::ErrorCreatingSwapChain;
}

void VulkanHandler::createDescriptorPool() {
	VkDescriptorPoolSize poolSize{
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = static_cast<uint32_t>(2)};
	VkDescriptorPoolCreateInfo poolInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.maxSets = static_cast<uint32_t>(2),
			.poolSizeCount = 1,
			.pPoolSizes = &poolSize};
	if (const VkResult result = vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool); result != VK_SUCCESS) {
		OWL_CORE_ERROR("failed to create descriptor pool ({})", resultString(result))
		state = State::ErrorCreatingDescriptorPool;
	}
}

void VulkanHandler::createDescriptorSets(const size_t size) {
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
	const VkDescriptorSetAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext = nullptr,
			.descriptorPool = descriptorPool,
			.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
			.pSetLayouts = layouts.data()};
	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (const auto result = vkAllocateDescriptorSets(logicalDevice, &allocInfo, descriptorSets.data()); result != VK_SUCCESS) {
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
		vkUpdateDescriptorSets(logicalDevice, 1, &descriptorWrite, 0, nullptr);
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
	if (const VkResult result = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pData.layout); result != VK_SUCCESS) {
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
			.polygonMode = VK_POLYGON_MODE_FILL,
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
			.colorBlendOp = {},
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
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
			.blendConstants = {0.f, 0.f, 0.f, 1.f}};
	VkDynamicState dynamicStates[] = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR};
	VkPipelineDynamicStateCreateInfo dynamicState{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.dynamicStateCount = 2,
			.pDynamicStates = dynamicStates};
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
			.pDepthStencilState = nullptr,
			.pColorBlendState = &colorBlending,
			.pDynamicState = &dynamicState,
			.layout = pData.layout,
			.renderPass = swapChain.renderPass,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = 0};

	OWL_CORE_TRACE("Vulkan pipeline: vkCreateGraphicsPipelines")
	if (const VkResult result = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pData.pipeLine); result != VK_SUCCESS) {
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
	if (!pipeLines.contains(id))
		return;
	if (pipeLines[id].pipeLine != nullptr)
		vkDestroyPipeline(logicalDevice, pipeLines[id].pipeLine, nullptr);
	if (pipeLines[id].layout != nullptr)
		vkDestroyPipelineLayout(logicalDevice, pipeLines[id].layout, nullptr);
	pipeLines.erase(id);
}

void VulkanHandler::createCommandPool() {
	VkCommandPoolCreateInfo poolInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = physicalDevice.queues.graphicsIndex};

	if (const VkResult result = vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create command pool ({}).", resultString(result))
		state = State::ErrorCreatingCommandPool;
	}
}

void VulkanHandler::createCommandBuffers() {
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	const VkCommandBufferAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = commandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = static_cast<uint32_t>(commandBuffers.size())};
	if (const VkResult result = vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data()); result != VK_SUCCESS) {
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

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		if (const VkResult result = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: failed to create image available semaphore {} ({}).", i, resultString(result))
			state = State::ErrorCreatingSyncObjects;
			return;
		}
		if (const VkResult result = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: failed to create render finish semaphore {} ({}).", i, resultString(result))
			state = State::ErrorCreatingSyncObjects;
			return;
		}
		if (const VkResult result = vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]); result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: failed to create synchronization in flight fence {} ({}).", i, resultString(result))
			state = State::ErrorCreatingSyncObjects;
			return;
		}
	}
}

void VulkanHandler::drawData(uint32_t vertexCount) const {
	if (state != State::Running)
		return;
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
	vkCmdDrawIndexed(getCurrentCommandBuffer(), vertexCount, 1, 0, 0, 0);
}

void VulkanHandler::beginFrame() {
	if (state != State::Running)
		return;
	vkWaitForFences(logicalDevice, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	if (const VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain.swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex); result != VK_SUCCESS) {
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			swapChain.recreate(physicalDevice.capabilities.currentExtent);
			return;
		}
		if (result != VK_SUBOPTIMAL_KHR) {
			OWL_CORE_ERROR("Vulkan: failed to aquire next image ({}).", resultString(result))
			state = State::ErrorAcquiringNextImage;
			return;
		}
		vkResetFences(logicalDevice, 1, &inFlightFences[currentFrame]);
	}

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
}

void VulkanHandler::endFrame() {
	if (state != State::Running)
		return;
	vkCmdEndRenderPass(getCurrentCommandBuffer());

	if (const VkResult result = vkEndCommandBuffer(getCurrentCommandBuffer()); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to end command buffer ({}).", resultString(result))
		state = State::ErrorEndCommandBuffer;
	}
}

void VulkanHandler::swapFrame() {
	if (state != State::Running)
		return;
	const VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	const VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	const VkSubmitInfo submitInfo{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext = nullptr,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = waitSemaphores,
			.pWaitDstStageMask = waitStages,
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffers[currentFrame],
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = signalSemaphores};

	if (const VkResult result = vkQueueSubmit(physicalDevice.queues.graphics, 1, &submitInfo, inFlightFences[currentFrame]); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to submit draw command buffer ({}).", resultString(result))
		state = State::ErrorSubmitingDrawCommand;
		return;
	}

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

	if (const VkResult result = vkQueuePresentKHR(physicalDevice.queues.present, &presentInfo); result != VK_SUCCESS) {
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || resize) {
			resize = false;
			swapChain.recreate(physicalDevice.capabilities.currentExtent);
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
	physicalDevice.updateSurfaceInformations();
	resize = true;
}

void VulkanHandler::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const {
	const VkCommandBufferAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = commandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1};

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

	constexpr VkCommandBufferBeginInfo beginInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			.pInheritanceInfo = nullptr};
	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

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

	vkQueueSubmit(physicalDevice.queues.graphics, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(physicalDevice.queues.graphics);

	vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
}

void VulkanHandler::createDescriptorSetLayout() {
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
	if (const auto result = vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create descriptor set layout ({}).", resultString(result))
		state = State::ErrorCreatingDescriptorSetLayout;
	}
}

void VulkanHandler::createUniformBuffers(const size_t size) {
	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

		vkMapMemory(logicalDevice, uniformBuffersMemory[i], 0, size, 0, &uniformBuffersMapped[i]);
	}
	createDescriptorSets(size);
}

void VulkanHandler::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory) const {
	const VkBufferCreateInfo bufferInfo{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.size = size,
			.usage = usage,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = nullptr};

	if (const VkResult result = vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan buffer: failed to create vertex buffer ({}).", resultString(result))
		return;
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

	const VkMemoryAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = nullptr,
			.allocationSize = memRequirements.size,
			.memoryTypeIndex = physicalDevice.findMemoryTypeIndex(memRequirements.memoryTypeBits, properties)};

	if (const VkResult result = vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan vertex buffer: failed to allocate memory buffer ({}).", resultString(result))
		return;
	}

	if (const VkResult result = vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan vertex buffer: failed to bind memory buffer ({}).", resultString(result))
	}
}

void VulkanHandler::setUniformData(const void *data, size_t size) const {
	memcpy(uniformBuffersMapped[currentFrame], data, size);
}

}// namespace owl::renderer::vulkan::internal
