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
	OWL_CORE_ASSERT(status, "Failed to initialize GLAD")

	OWL_CORE_INFO("OpenGL Info:")
	OWL_CORE_INFO("  Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)))
	OWL_CORE_INFO("  Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)))
	OWL_CORE_INFO("  Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)))
#ifdef OWL_ENABLE_ASSERTS
	int versionMajor,versionMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
	glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
	OWL_CORE_ASSERT(versionMajor > 4 || (versionMajor ==4 && versionMinor >= 5), "Owl Engin requires at least OpenGL version 4.5")
#endif
}

void GraphContext::SwapBuffers() {
	glfwSwapBuffers(windowHandle);
}

}// namespace owl::renderer::opengl
