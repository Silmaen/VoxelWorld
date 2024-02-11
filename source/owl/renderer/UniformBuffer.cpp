/**
 * @file UniformBuffer.cpp
 * @author Silmaen
 * @date 02/01/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include <magic_enum.hpp>

#include "Renderer.h"
#include "UniformBuffer.h"
#include "null/UniformBuffer.h"
#include "opengl/UniformBuffer.h"
#include "opengl_legacy/UniformBuffer.h"
#include "vulkan/UniformBuffer.h"

namespace owl::renderer {

shared<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding, const std::string &renderer) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::Null:
			return mk_shared<null::UniformBuffer>(size, binding);
		case RenderAPI::Type::OpenGL:
			return mk_shared<opengl::UniformBuffer>(size, binding);
		case RenderAPI::Type::OpenglLegacy:
			return mk_shared<opengl_legacy::UniformBuffer>(size, binding);
		case RenderAPI::Type::Vulkan:
			return mk_shared<vulkan::UniformBuffer>(size, binding, renderer);
	}
	OWL_CORE_ERROR("Unknown API Type!")
	return nullptr;
}
UniformBuffer::~UniformBuffer() = default;

}// namespace owl::renderer
