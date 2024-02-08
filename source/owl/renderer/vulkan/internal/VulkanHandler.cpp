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
DrawData data;

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
		createCommandBuffer();
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
		OWL_CORE_TRACE("Vulkan: Just for debugging, create a triangle pipeline.")
		std::vector<uint32_t> bob{};
		data.init({}, "renderer2D", bob, "triangle");
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

	if (renderFinishedSemaphore != nullptr) {
		vkDestroySemaphore(logicalDevice, renderFinishedSemaphore, nullptr);
		OWL_CORE_TRACE("Vulkan: renderFinishedSemaphore destroyed.")
	}
	if (imageAvailableSemaphore != nullptr) {
		vkDestroySemaphore(logicalDevice, imageAvailableSemaphore, nullptr);
		OWL_CORE_TRACE("Vulkan: imageAvailableSemaphore destroyed.")
	}
	if (inFlightFence != nullptr) {
		vkDestroyFence(logicalDevice, inFlightFence, nullptr);
		OWL_CORE_TRACE("Vulkan: inFlightFence destroyed.")
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
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Owl";
	appInfo.pEngineName = "OwlEngine";
	appInfo.apiVersion = VK_API_VERSION_1_3;

	// Supported Instance extensions & Layer
	{
		// Get extensions supported by the instance and store for later use
		uint32_t extCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
		if (extCount > 0) {
			std::vector<VkExtensionProperties> extensions(extCount);
			const VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front());
			if (result == VK_SUCCESS) {
				for (VkExtensionProperties &extension: extensions) {
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
		const char *validationLayerName = "VK_LAYER_KHRONOS_validation";
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
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessagerCI{};
	debugUtilsMessagerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsMessagerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugUtilsMessagerCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
	debugUtilsMessagerCI.pfnUserCallback = debugUtilsMessageCallback;
	const VkResult result = vkCreateDebugUtilsMessengerEXT(instance, &debugUtilsMessagerCI, nullptr, &debugUtilsMessenger);
	if (result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: Error while setup debugging ({})", resultString(result))
		state = State::ErrorSetupDebugging;
	}
}

void VulkanHandler::createSurface() {
	auto gc = dynamic_cast<vulkan::GraphContext *>(core::Application::get().getWindow().getGraphContext());
	const VkResult result = gc->createSurface(instance);
	if (result != VK_SUCCESS) {
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
	for (const uint32_t queue: physicalDevice.queues.getUniqueIndices()) {
		VkDeviceQueueCreateInfo deviceQueueCI{};
		deviceQueueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCI.queueCount = 1;
		deviceQueueCI.queueFamilyIndex = queue;
		float queuePriority = 1.0f;
		deviceQueueCI.pQueuePriorities = &queuePriority;
		deviceQueuesCI.push_back(deviceQueueCI);
	}
	VkDeviceCreateInfo deviceCI{};
	deviceCI.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCI.pQueueCreateInfos = deviceQueuesCI.data();
	deviceCI.queueCreateInfoCount = static_cast<uint32_t>(deviceQueuesCI.size());

	deviceCI.pEnabledFeatures = &physicalDevice.enabledFeatures;

	deviceCI.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	deviceCI.ppEnabledExtensionNames = deviceExtensions.data();

	if (validationPresent) {
		const char *validationLayerName = "VK_LAYER_KHRONOS_validation";
		deviceCI.enabledLayerCount = 1;
		deviceCI.ppEnabledLayerNames = &validationLayerName;
	} else {
		deviceCI.enabledLayerCount = 0;
	}
	const VkResult result = vkCreateDevice(physicalDevice.device, &deviceCI, nullptr, &logicalDevice);
	if (result != VK_SUCCESS) {
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
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(2);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(2);
	const VkResult result = vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool);
	if (result != VK_SUCCESS) {
		OWL_CORE_ERROR("failed to create descriptor pool ({})", resultString(result))
		state = State::ErrorCreatingDescriptorPool;
	}
}

VulkanHandler::PipeLineData VulkanHandler::getPipeline(int32_t id) const {
	if (state == State::Running || !pipeLines.contains(id))
		return {};
	return pipeLines.at(id);
}

int32_t VulkanHandler::pushPipeline(const std::string &pipeLineName, std::vector<VkPipelineShaderStageCreateInfo> &shaderStages) {
	PipeLineData pData;
	// PipeLine Layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	{
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
	}
	if (const VkResult result = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pData.layout); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: Shader: failed to create pipeline layout {} ({})", pipeLineName, resultString(result))
		state = State::ErrorCreatingPipelineLayout;
		return -1;
	}

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	VkPipelineViewportStateCreateInfo viewportState{};
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	VkPipelineMultisampleStateCreateInfo multisampling{};
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	VkPipelineColorBlendStateCreateInfo colorBlending{};
	VkPipelineDynamicStateCreateInfo dynamicState{};
	{
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;

		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkDynamicState dynamicStates[] = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR};

		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = 2;
		dynamicState.pDynamicStates = dynamicStates;
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = pData.layout;
	pipelineInfo.renderPass = swapChain.renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

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
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = physicalDevice.queues.graphicsIndex;

	if (const VkResult result = vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create command pool ({}).", resultString(result))
		state = State::ErrorCreatingCommandPool;
	}
}

void VulkanHandler::createCommandBuffer() {
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (const VkResult result = vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer); result != VK_SUCCESS) {
		OWL_CORE_ERROR("failed to allocate command buffers ({}).", resultString(result))
		state = State::ErrorCreatingCommandBuffer;
	}
}

void VulkanHandler::createSyncObjects() {
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	if (const VkResult result = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create image available semaphore ({}).", resultString(result))
		state = State::ErrorCreatingSyncObjects;
		return;
	}
	if (const VkResult result = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create render finish semaphore ({}).", resultString(result))
		state = State::ErrorCreatingSyncObjects;
		return;
	}
	if (const VkResult result = vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFence); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to create synchronization in flight fence ({}).", resultString(result))
		state = State::ErrorCreatingSyncObjects;
	}
}

void VulkanHandler::drawFrame() {
	if (state != State::Running)
		return;
	// todo: Will be replaced by layers draw...
	if (!pipeLines.empty()) {
		data.bind();

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapChain.swapChainExtent.width);
		viewport.height = static_cast<float>(swapChain.swapChainExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = swapChain.swapChainExtent;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	}
}

void VulkanHandler::beginFrame() {
	if (state != State::Running)
		return;
	vkWaitForFences(logicalDevice, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(logicalDevice, 1, &inFlightFence);

	{
		const VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain.swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
		if (result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: failed to aquire next image ({}).", resultString(result))
			state = State::ErrorAcquiringNextImage;
			return;
		}
	}

	{
		const VkResult result = vkResetCommandBuffer(commandBuffer, /*VkCommandBufferResetFlagBits*/ 0);
		if (result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: failed to reset recording command buffer ({}).", resultString(result))
			state = State::ErrorResetCommandBuffer;
			return;
		}
	}
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	{
		const VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		if (result != VK_SUCCESS) {
			OWL_CORE_ERROR("Vulkan: failed to begin recording command buffer ({}).", resultString(result))
			state = State::ErrorBeginCommandBuffer;
			return;
		}
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChain.renderPass;
	renderPassInfo.framebuffer = swapChain.swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = swapChain.swapChainExtent;

	VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	// todo: should disapear!!!
	drawFrame();
}

void VulkanHandler::endFrame() {
	if (state != State::Running)
		return;
	vkCmdEndRenderPass(commandBuffer);

	if (const VkResult result = vkEndCommandBuffer(commandBuffer); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to end command buffer ({}).", resultString(result))
		state = State::ErrorEndCommandBuffer;
	}
}

void VulkanHandler::swapFrame() {
	if (state != State::Running)
		return;
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	const VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
	constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	const VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (const VkResult result = vkQueueSubmit(physicalDevice.queues.graphics, 1, &submitInfo, inFlightFence); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to submit draw command buffer ({}).", resultString(result))
		state = State::ErrorSubmitingDrawCommand;
		return;
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	const VkSwapchainKHR swapChains[] = {swapChain.swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	if (const VkResult result = vkQueuePresentKHR(physicalDevice.queues.present, &presentInfo); result != VK_SUCCESS) {
		OWL_CORE_ERROR("Vulkan: failed to present queue ({}).", resultString(result))
		state = State::ErrorPresentingQueue;
	}
}

void VulkanHandler::bindPipeline(const int32_t id) {
	if (state != State::Running)
		return;
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLines[id].pipeLine);
}

}// namespace owl::renderer::vulkan::internal
