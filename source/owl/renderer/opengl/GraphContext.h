/**
 * @file GraphContext.h
 * @author Silmen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../GraphContext.h"
#include "core/external/glfw3.h"

namespace owl::renderer::opengl {

/**
 * @brief Class GraphContext
 */
class OWL_API GraphContext : public ::owl::renderer::GraphContext {
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
	 * @brief Initialize the context
	 */
	void Init() override;

	/**
	 * @brief Doo the buffer swap
	 */
	void SwapBuffers() override;

private:
	/// Link to the parent window
	GLFWwindow *windowHandle;
};

}// namespace owl::renderer::opengl
