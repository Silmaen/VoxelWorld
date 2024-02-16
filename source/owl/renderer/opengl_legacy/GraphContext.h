/**
 * @file GraphContext.h
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../GraphContext.h"
#include "core/external/glfw3.h"

namespace owl::renderer::opengl_legacy {

/**
 * @brief Class GraphContext.
 */
class OWL_API GraphContext final : public ::owl::renderer::GraphContext {
public:
	GraphContext(const GraphContext &) = delete;
	GraphContext(GraphContext &&) = delete;
	GraphContext &operator=(const GraphContext &) = delete;
	GraphContext &operator=(GraphContext &&) = delete;

	/**
	 * @brief Default constructor.
	 */
	explicit GraphContext(GLFWwindow *window);

	/**
	 * @brief Destructor.
	 */
	~GraphContext() override = default;//---UNCOVER---

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
	Version getVersion() const override;

private:
	/// Link to the parent window.
	GLFWwindow *windowHandle;
	/// OpenGL version
	int version;
};

}// namespace owl::renderer::opengl_legacy
