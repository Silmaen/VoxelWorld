/**
 * @file DrawData.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "DrawData.h"

#include "renderer/Renderer.h"

namespace owl::renderer::vulkan {

DrawData::~DrawData() = default;

void DrawData::init([[maybe_unused]] const BufferLayout &layout_, const std::string &renderer, [[maybe_unused]] std::vector<uint32_t> &indices, const std::string &shaderName) {
	setShader(shaderName, renderer);
}
void DrawData::setShader(const std::string &shaderName, const std::string &renderer) {
	auto &shLib = Renderer::getShaderLibrary();
	if (!shLib.exists(shaderName, renderer))
		shLib.addFromStandardPath(shaderName, renderer);
	shader = static_pointer_cast<Shader>(shLib.get(shaderName, renderer));
}

void DrawData::bind() const {
	shader->bind();
}

}// namespace owl::renderer::vulkan
