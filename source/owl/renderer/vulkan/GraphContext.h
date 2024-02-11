/**
 * @file GraphContext.h
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../GraphContext.h"
#include "core/external/glfw3.h"

namespace owl::renderer::vulkan {

/**
 * @brief Class GraphContext.
 */
class OWL_API GraphContext final : public ::owl::renderer::GraphContext {
public:
	GraphContext(const GraphContext &) = delete;
	GraphContext(GraphContext &&) = delete;

	/**
	 * @brief Default constructor.
	 */
	explicit GraphContext(GLFWwindow *window);

	/**
	 * @brief Destructor.
	 */
	~GraphContext() override;

	/**
	 * @brief Initialize the context.
	 */
	void init() override;

	/**
	 * @brief Do the buffer swap.
	 */
	void swapBuffers() override;
	/**
	 * @brief Get version number of the backend API.
	 * @return The version number.
	 */
	[[nodiscard]] Version getVersion() const override;

	/**
	 * @brief Create the Window surface.
	 * @param instance The Vulkan instance
	 * @return The operation result.
	 */
	VkResult createSurface(VkInstance instance);

	/**
	 * @brief Destroy the Window surface.
	 * @param instance The Vulkan instance
	 */
	void destroySurface(VkInstance instance);

	/**
	 * \brief Access to the vulkn surface.
	 * \return The vulkan surface.
	 */
	[[nodiscard]] VkSurfaceKHR getSurface() const { return surface; }
	/**
	 * @brief Wait for device ready.
	 */
	void waitIdle() override;

private:
	/// The window.
	GLFWwindow *wnd = nullptr;
	/// the presenttin surface.
	VkSurfaceKHR surface = nullptr;
};

}// namespace owl::renderer::vulkan
