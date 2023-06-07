/**
 * @file GraphContext.cpp
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "gl_21/glad.h"

#include "GraphContext.h"

namespace owl::renderer::opengl_legacy {

GraphContext::GraphContext(GLFWwindow *window) : windowHandle(window) {
	OWL_CORE_ASSERT(window, "Windows handle is nullptr")
}

void GraphContext::init() {
	OWL_PROFILE_FUNCTION()

	glfwMakeContextCurrent(windowHandle);
	[[maybe_unused]] int status = gl_21::gladLoadGLLoader(reinterpret_cast<gl_21::GLADloadproc>(glfwGetProcAddress));
	OWL_CORE_ASSERT(status, "Failed to initialize GLAD")

	OWL_CORE_INFO("OpenGL Info:")
	OWL_CORE_INFO("  Vendor: {}", reinterpret_cast<const char *>(gl_21::glGetString(GL_VENDOR)))
	OWL_CORE_INFO("  Renderer: {}", reinterpret_cast<const char *>(gl_21::glGetString(GL_RENDERER)))
	OWL_CORE_INFO("  Version: {}", reinterpret_cast<const char *>(gl_21::glGetString(GL_VERSION)))
}

void GraphContext::swapBuffers() {
	OWL_PROFILE_FUNCTION()

	glfwSwapBuffers(windowHandle);
}

}// namespace owl::renderer::opengl_legacy
