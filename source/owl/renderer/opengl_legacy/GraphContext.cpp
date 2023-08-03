/**
 * @file GraphContext.cpp
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */


#include "glad21/glad.h"

#include "GraphContext.h"

namespace owl::renderer::opengl_legacy {

GraphContext::GraphContext(GLFWwindow *window) : windowHandle(window) {
	OWL_CORE_ASSERT(window, "Windows handle is nullptr")
}

void GraphContext::init() {
	OWL_PROFILE_FUNCTION()

	glfwMakeContextCurrent(windowHandle);
	[[maybe_unused]] int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	OWL_CORE_ASSERT(status, "Failed to initialize GLAD")

	OWL_CORE_INFO("OLdOpengl Renderer Initiated.")
	OWL_CORE_INFO("Device Info:")
	OWL_CORE_INFO("  Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)))
	OWL_CORE_INFO("  Renderer: {}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)))
	OWL_CORE_INFO("  Version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)))
	int32_t textureUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
	OWL_CORE_INFO(" Max texture slot per Shader: {}", textureUnits)
}

void GraphContext::swapBuffers() {
	OWL_PROFILE_FUNCTION()

	glfwSwapBuffers(windowHandle);
}

}// namespace owl::renderer::opengl_legacy
