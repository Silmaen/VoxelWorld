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

	OWL_CORE_INFO("OpenGL Info:");
	OWL_CORE_INFO("  Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	OWL_CORE_INFO("  Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	OWL_CORE_INFO("  Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
}

void GraphContext::SwapBuffers() {
	glfwSwapBuffers(windowHandle);
}

}// namespace owl::renderer::opengl
