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
class OWL_API GraphContext : public ::owl::renderer::GraphContext {
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
	 * @brief Doo the buffer swap.
	 */
	void swapBuffers() override;
	/**
	 * @brief Get version number of the backend API.
	 * @return The version number.
	 */
	[[nodiscard]] Version getVersion() const override { return {1, 3}; }
};

}// namespace owl::renderer::vulkan
