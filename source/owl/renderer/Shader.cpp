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

shared<Shader> Shader::create(const std::string &iShaderName, const std::string &iRenderer) {
	const auto type = RenderCommand::getApi();
	std::filesystem::path shaderDir;
	if (RenderCommand::requireInit()) {
		shaderDir = core::Application::get().getAssetDirectory() / "shaders";
		if (!iRenderer.empty()) {
			shaderDir /= iRenderer;
			if (type == RenderAPI::Type::OpenGL)
				shaderDir /= "opengl";
			else if (type == RenderAPI::Type::OpenglLegacy)
				shaderDir /= "opengl_legacy";
			else if (type == RenderAPI::Type::Vulkan)
				shaderDir /= "vulkan";
		}
	}
	return create(iShaderName, iRenderer, shaderDir);
}

shared<Shader> Shader::create(const std::string &iShaderName, const std::string &iRenderer,
							  const std::filesystem::path &iFile) {
	std::vector<std::filesystem::path> sources;
	if (is_directory(iFile)) {
		for (const auto &f: std::filesystem::directory_iterator(iFile)) {
			if (f.path().stem() != iShaderName)
				continue;
			sources.push_back(f);
		}
		if (sources.empty()) {
			OWL_CORE_WARN("Not able to find Shader {} in {}", iShaderName, iFile.string())
			return nullptr;
		}
	} else { sources = {iFile}; }
	OWL_CORE_TRACE("Try to create shader {} for renderer {} / API {}.", iShaderName, iRenderer,
				   magic_enum::enum_name(RenderCommand::getApi()))
	switch (RenderCommand::getApi()) {
		case RenderAPI::Type::Null:
			return mkShared<null::Shader>(iShaderName, iRenderer, sources);
		case RenderAPI::Type::OpenGL:
			return mkShared<opengl::Shader>(iShaderName, iRenderer, sources);
		case RenderAPI::Type::OpenglLegacy:
			return mkShared<opengl_legacy::Shader>(iShaderName, iRenderer, sources);
		case RenderAPI::Type::Vulkan:
			return mkShared<vulkan::Shader>(iShaderName, iRenderer, sources);
	}
	OWL_CORE_ERROR("Unknown API Type!")
	return nullptr;
}

Shader::~Shader() = default;

}// namespace owl::renderer
