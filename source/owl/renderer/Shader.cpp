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
#include "opengl/Shader.h"
#include <magic_enum.hpp>

namespace owl::renderer {

shared<Shader> Shader::create(const std::string &shaderName, const std::string &vertexSrc, const std::string &fragmentSrc) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::None:
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ASSERT(false, "Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::OpenGL:
			return mk_shared<opengl::Shader>(shaderName, vertexSrc, fragmentSrc);
	}
	OWL_CORE_ASSERT(false, "Unknown API Type!")
	return nullptr;
}

shared<Shader> Shader::create(const std::string &shaderName) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::None:
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ASSERT(false, "Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::OpenGL: {
			std::vector<std::filesystem::path> sources;
			auto shaderDir = core::Application::get().getAssetDirectory() / "shaders";
			for (const auto &file: std::filesystem::directory_iterator(shaderDir)) {
				if (file.path().stem().string() == shaderName)
					sources.push_back(file);
			}
			if (sources.empty()) {
				OWL_CORE_WARN("Not able to find Shader {} in directory {}", shaderName, shaderDir.string())
				return nullptr;
			}
			return mk_shared<opengl::Shader>(shaderName, sources);
		}
	}
	OWL_CORE_ASSERT(false, "Unknown API Type!")
	return nullptr;
}

shared<Shader> Shader::create(const std::filesystem::path &file) {
	auto type = Renderer::getAPI();
	switch (type) {
		case RenderAPI::Type::None:
		case RenderAPI::Type::Vulkan:
			OWL_CORE_ASSERT(false, "Render API {} is not yet supported", magic_enum::enum_name(type))
			return nullptr;
		case RenderAPI::Type::OpenGL: {
			std::string shaderName = file.stem().string();
			if (is_directory(file)) {
				std::vector<std::filesystem::path> sources;
				for (const auto &f: file)
					sources.push_back(f);
				if (sources.empty()) {
					OWL_CORE_WARN("Not able to find Shader in {}", file.string())
					return nullptr;
				}

				return mk_shared<opengl::Shader>(shaderName, sources);
			} else {
				return mk_shared<opengl::Shader>(shaderName, std::vector<std::filesystem::path>{file});
			}
		}
	}
	OWL_CORE_ASSERT(false, "Unknown API Type!")
	return nullptr;
}

}// namespace owl::renderer
