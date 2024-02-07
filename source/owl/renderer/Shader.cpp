/**
 * @file Shader.cpp
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Renderer.h"
#include "Shader.h"
#include "core/Application.h"
#include "null/Shader.h"
#include "opengl/Shader.h"
#include "opengl_legacy/Shader.h"
#include "vulkan/Shader.h"

namespace owl::renderer {

shared<Shader> Shader::create(const std::string &shaderName, const std::string &renderer) {
	const auto type = Renderer::getAPI();
	std::filesystem::path shaderDir;
	if (RenderAPI::requireInit()) {
		shaderDir = core::Application::get().getAssetDirectory() / "shaders";
		if (!renderer.empty()) {
			shaderDir /= renderer;
			if (type == RenderAPI::Type::OpenGL)
				shaderDir /= "opengl";
			else if (type == RenderAPI::Type::OpenglLegacy)
				shaderDir /= "opengl_legacy";
			else if (type == RenderAPI::Type::Vulkan)
				shaderDir /= "vulkan";
		}
	}
	return create(shaderName, renderer, shaderDir);
}

shared<Shader> Shader::create(const std::string &shaderName, const std::string &renderer, const std::filesystem::path &file) {
	std::vector<std::filesystem::path> sources;
	if (is_directory(file)) {
		for (const auto &f: std::filesystem::directory_iterator(file)) {
			if (f.path().stem() != shaderName)
				continue;
			sources.push_back(f);
		}
		if (sources.empty()) {
			OWL_CORE_WARN("Not able to find Shader {} in {}", shaderName, file.string())
			return nullptr;
		}
	} else {
		sources = {file};
	}
	OWL_CORE_TRACE("Try to create shader {} for renderer {} / API {}.", shaderName, renderer, magic_enum::enum_name(Renderer::getAPI()))
	switch (Renderer::getAPI()) {
		case RenderAPI::Type::Null:
			return mk_shared<null::Shader>(shaderName, renderer, sources);
		case RenderAPI::Type::OpenGL:
			return mk_shared<opengl::Shader>(shaderName, renderer, sources);
		case RenderAPI::Type::OpenglLegacy:
			return mk_shared<opengl_legacy::Shader>(shaderName, renderer, sources);
		case RenderAPI::Type::Vulkan:
			return mk_shared<vulkan::Shader>(shaderName, renderer, sources);
	}
	OWL_CORE_ERROR("Unknown API Type!")
	return nullptr;
}
Shader::~Shader() = default;

}// namespace owl::renderer
