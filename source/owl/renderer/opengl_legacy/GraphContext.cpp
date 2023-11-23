/**
 * @file GraphContext.cpp
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "core/external/opengl21.h"

#include "GraphContext.h"

namespace owl::renderer::opengl_legacy {

GraphContext::GraphContext(GLFWwindow *window) : windowHandle(window) {
	OWL_CORE_ASSERT(window, "Windows handle is nullptr")
}

void GraphContext::init() {
	OWL_PROFILE_FUNCTION()

	glfwMakeContextCurrent(windowHandle);
#ifdef OLD_GLAD
	version = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
#else
	version = gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress));
#endif
	OWL_CORE_ASSERT(version, "Failed to initialize GLAD")

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

GraphContext::Version GraphContext::getVersion() const {
#ifdef OLD_GLAD
	return {GLVersion.major, GLVersion.minor};
#else
	return {GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version)};
#endif
}

}// namespace owl::renderer::opengl_legacy
