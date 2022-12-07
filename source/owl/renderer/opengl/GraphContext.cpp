/**
 * @file GraphContext.cpp
 * @author Silmen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include <glad/glad.h>

#include "GraphContext.h"

namespace owl::renderer::opengl {

GraphContext::GraphContext(GLFWwindow *window) : windowHandle(window) {
	OWL_CORE_ASSERT(window, "Windows handle is nullptr")
}

void GraphContext::Init() {
	glfwMakeContextCurrent(windowHandle);
	[[maybe_unused]] int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	OWL_CORE_ASSERT(status, "Failed to initialize GLAD");
}

void GraphContext::SwapBuffers() {
	glfwSwapBuffers(windowHandle);
}

}// namespace owl::renderer::opengl
