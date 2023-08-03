/**
 * @file GraphContext.cpp
 * @author Silmaen
 * @date 30/07/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */


#include "GraphContext.h"

namespace owl::renderer::null {

GraphContext::GraphContext(GLFWwindow *window) : windowHandle(window) {
	OWL_CORE_ASSERT(window, "Windows handle is nullptr")
}

void GraphContext::init() {
	OWL_PROFILE_FUNCTION()

	glfwMakeContextCurrent(windowHandle);
}

void GraphContext::swapBuffers() {
	OWL_PROFILE_FUNCTION()

	glfwSwapBuffers(windowHandle);
}

}// namespace owl::renderer::null
