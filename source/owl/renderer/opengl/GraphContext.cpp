/**
 * @file GraphContext.cpp
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "core/external/opengl46.h"

#include "GraphContext.h"

namespace owl::renderer::opengl {

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
	OWL_CORE_ASSERT(version, "Failed to initialize GLAD for OpenGL")

	OWL_CORE_INFO("OpenGL GraphContext Initiated.")
	OWL_CORE_INFO("Device Info:")
	OWL_CORE_INFO("  Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)))
	OWL_CORE_INFO("  Renderer: {}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)))
	OWL_CORE_INFO("  Version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)))
	int32_t textureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &textureUnits);
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

}// namespace owl::renderer::opengl
