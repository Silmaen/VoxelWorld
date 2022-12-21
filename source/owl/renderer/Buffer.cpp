/**
 * @file Buffer.cpp
 * @author Silmen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Buffer.h"
#include "Renderer.h"
#include "opengl/Buffer.h"
#include <magic_enum.hpp>

namespace owl::renderer {


shrd<VertexBuffer> VertexBuffer::create( uint32_t size){
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::None:
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ASSERT(false,"Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::OpenGL:
			return mk_shrd<opengl::VertexBuffer>(size);
	}
	OWL_CORE_ASSERT(false, "Unknown API Type!")
	return nullptr;
}

shrd<VertexBuffer> VertexBuffer::create(float *vertices, uint32_t size) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::None:
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ASSERT(false,"Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::OpenGL:
			return mk_shrd<opengl::VertexBuffer>(vertices, size);
	}
	OWL_CORE_ASSERT(false, "Unknown API Type!")
	return nullptr;
}

shrd<IndexBuffer> IndexBuffer::create(uint32_t *indices, uint32_t size) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::None:
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ASSERT(false,"Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::OpenGL:
			return mk_shrd<opengl::IndexBuffer>(indices, size);
	}
	OWL_CORE_ASSERT(false, "Unknown API Type!")
	return nullptr;
}

}// namespace owl::renderer
