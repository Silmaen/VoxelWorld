/**
 * @file GraphContext.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */


#include "GraphContext.h"

namespace owl::renderer::vulkan {

GraphContext::GraphContext(GLFWwindow *){};

GraphContext::~GraphContext() = default;

void GraphContext::swapBuffers() {}

void GraphContext::init() {}

}// namespace owl::renderer::vulkan
