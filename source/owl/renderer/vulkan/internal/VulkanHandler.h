/**
 * @file VulkanHandler.h
 * @author Silmaen
 * @date 30/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "SwapChain.h"
#include "VulkanCore.h"

#include <backends/imgui_impl_vulkan.h>
#include <map>

/**
 * @brief Internal functions of the vulkan renderer.
 */
namespace owl::renderer::vulkan::internal {

/**
 * @brief Class VulkanHandler.
 */
class VulkanHandler final {
public:
	VulkanHandler(const VulkanHandler &) = delete;
	VulkanHandler(VulkanHandler &&) = delete;
	VulkanHandler &operator=(const VulkanHandler &) = delete;
	VulkanHandler &operator=(VulkanHandler &&) = delete;

	/**
	 * @brief Destructor.
	 */
	~VulkanHandler();

	/**
	 * @brief Handler for vulkan objects
	 * @return Vulcan handler
	 */
	static VulkanHandler &get() {
		static VulkanHandler handler;
		return handler;
	}

	/**
	 * @brief Initialize the vulkan handler.
	 */
	void initVulkan();

	/**
	 * @brief Release the vulkan handler.
	 */
	void release();

	/// List of handler states.
	enum struct State {
		/// Not initialized of closed.
		Uninitialized,
		/// Initialized and ready.
		Running,
		/// Encounter an error while creating the core of vulkan.
		ErrorCreatingCore,
		ErrorCreatingSwapChain,
		ErrorCreatingImagesView,
		ErrorCreatingRenderPass,
		ErrorCreatingPipelineLayout,
		ErrorCreatingPipeline,
		ErrorCreatingDescriptorPool,
		ErrorCreatingCommandPool,
		ErrorCreatingCommandBuffer,
		ErrorSubmitingDrawCommand,
		ErrorPresentingQueue,
		ErrorAcquiringNextImage,
		ErrorResetCommandBuffer,
		ErrorBeginCommandBuffer,
		ErrorEndCommandBuffer,
		ErrorCreatingSyncObjects,
		ErrorCreatingDescriptorSet,
		ErrorCreatingDescriptorSetLayout,
	};

	/**
	 * @brief Gets the current state of the handler.
	 * @return The state of the handler.
	 */
	[[nodiscard]] const State &getState() const { return state; }

	/**
	 * @brief Define a new state for the vulkan handler
	 * @param st The new state.
	 */
	void setState(const State &st) { state = st; }

	/**
	 * @brief The vulkan version.
	 * @return The actual Vulkan version
	 */
	[[nodiscard]] int getVersion() const { return version; }

	/**
	 * @brief Activate the validation layer, if not already initialized.
	 */
	void activateValidation() {
		if (state == State::Uninitialized) validation = true;
	}

	[[nodiscard]] ImGui_ImplVulkan_InitInfo toImGuiInfo() const;

	[[nodiscard]] VkRenderPass getRenderPath() const { return swapChain.renderPass; }

	[[nodiscard]] VkCommandBuffer getCurrentCommandBuffer() const;


	// Pipelines management.
	struct PipeLineData {
		VkPipeline pipeLine = nullptr;
		VkPipelineLayout layout = nullptr;
	};
	[[nodiscard]] PipeLineData getPipeline(int32_t id) const;
	int32_t pushPipeline(const std::string &pipeLineName, std::vector<VkPipelineShaderStageCreateInfo> &shaderStages, VkPipelineVertexInputStateCreateInfo vertexInputInfo);
	void popPipeline(int32_t id);
	void bindPipeline(int32_t id);

	// Command buffer data
	VkCommandPool commandPool{nullptr};
	std::vector<VkCommandBuffer> commandBuffers{nullptr};
	void beginBatch();
	void endBatch();
	bool inBatch = false;
	bool firstBatch = true;

	void beginFrame();
	void endFrame();

	void swapFrame();

	void drawData(uint32_t vertexCount, bool indexed = true);
	void setClearColor(const glm::vec4 &color);
	void clear();

	void setResize();
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory) const;

	void createUniformBuffers(size_t size);
	void setUniformData(const void *data, size_t size) const;

private:
	/**
	 * @brief Default Constructor.
	 */
	VulkanHandler();

	/**
	 * @brief Create the instance.
	 */
	void createCore();
	void createSwapChain();
	void createDescriptorPool();
	void createCommandPool();
	void createCommandBuffers();
	void createSyncObjects();
	void createDescriptorSetLayout();


	/// The current state of the handler.
	State state = State::Uninitialized;
	/// Loaded version.
	int version = 0;
	/// Enable Validation layers.
	bool validation = false;
	bool resize = false;

	/// The swapchain.
	SwapChain swapChain;

	VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

	/// List of piplines.
	std::map<int32_t, PipeLineData> pipeLines;


	/// Main FrameBuffers
	std::vector<VkSemaphore> imageAvailableSemaphores{nullptr};
	std::vector<VkSemaphore> renderFinishedSemaphores{nullptr};
	std::vector<VkFence> inFlightFences{nullptr};
	int currentFrame = 0;

	uint32_t imageIndex = 0;


	// Uniform data
	VkDescriptorSetLayout descriptorSetLayout{nullptr};
	VkDescriptorPool descriptorPool{nullptr};
	void createDescriptorSets(size_t size);
	std::vector<VkDescriptorSet> descriptorSets;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void *> uniformBuffersMapped;
};

}// namespace owl::renderer::vulkan::internal
