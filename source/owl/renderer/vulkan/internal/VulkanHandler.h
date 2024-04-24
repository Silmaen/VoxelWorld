/**
 * @file VulkanHandler.h
 * @author Silmaen
 * @date 30/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "VulkanCore.h"

#include <backends/imgui_impl_vulkan.h>
#include <renderer/vulkan/Framebuffer.h>

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
	[[nodiscard]] const State &getState() const { return m_state; }

	/**
	 * @brief Define a new state for the vulkan handler
	 * @param[in] iState The new state.
	 */
	void setState(const State &iState) { m_state = iState; }

	/**
	 * @brief The vulkan version.
	 * @return The actual Vulkan version
	 */
	[[nodiscard]] int getVersion() const { return m_version; }

	/**
	 * @brief Activate the validation layer, if not already initialized.
	 */
	void activateValidation() {
		if (m_state == State::Uninitialized)
			m_validation = true;
	}

	[[nodiscard]] ImGui_ImplVulkan_InitInfo toImGuiInfo(std::vector<VkFormat> &ioFormats);

	[[nodiscard]] VkRenderPass getGlobalRenderPass() const { return m_swapChain->getRenderPass(); }

	[[nodiscard]] VkCommandBuffer getCurrentCommandBuffer() const;

	void clear() const;

	/**
	 * @brief Informations about pipelines.
	 */
	struct PipeLineData {
		VkPipeline pipeLine = nullptr;
		VkPipelineLayout layout = nullptr;
	};

	[[nodiscard]] PipeLineData getPipeline(int32_t iId) const;

	int32_t pushPipeline(const std::string &iPipeLineName, std::vector<VkPipelineShaderStageCreateInfo> &iShaderStages,
	                     VkPipelineVertexInputStateCreateInfo iVertexInputInfo);

	// Command buffer data
	bool inBatch = false;
	bool firstBatch = true;

	void popPipeline(int32_t iId);

	void bindPipeline(int32_t iId);

	void beginBatch();

	void endBatch();

	void beginFrame();

	void endFrame();

	void swapFrame();

	void drawData(uint32_t iVertexCount, bool iIndexed = true);

	void setClearColor(const glm::vec4 &iColor);

	void setResize();

	[[nodiscard]] uint32_t getCurrentFrameIndex() const;

	void bindFramebuffer(Framebuffer *iFrameBuffer);
	void unbindFramebuffer();
	[[nodiscard]] std::string getCurrentFrameBufferName() const;

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

	/// The current state of the handler.
	State m_state = State::Uninitialized;
	/// Loaded version.
	int m_version = 0;
	/// Enable Validation layers.
	bool m_validation = false;
	bool m_resize = false;
	VkRenderPass m_ImGuiRenderPass{};

	/// The swapchain (main framebuffer).
	uniq<Framebuffer> m_swapChain;
	/// The active framebuffer.
	Framebuffer *m_currentframebuffer = nullptr;

	glm::vec4 m_clearColor = {0.0f, 0.0f, 0.0f, 1.0f};

	/// List of piplines.
	std::map<int32_t, PipeLineData> m_pipeLines;

};
}// namespace owl::renderer::vulkan::internal
