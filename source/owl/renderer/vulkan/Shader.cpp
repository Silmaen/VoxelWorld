/**
 * @file Shader.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Shader.h"
#include "core/utils/FileUtils.h"
#include "internal/VulkanHandler.h"

#include <vulkan/vulkan_core.h>

namespace owl::renderer::vulkan {

Shader::Shader(const std::string &shaderName, const std::string &renderer, const std::string &vertexSrc, const std::string &fragmentSrc) : ::owl::renderer::Shader{shaderName, renderer} {
	OWL_PROFILE_FUNCTION()

	createShader({{ShaderType::Vertex, vertexSrc}, {ShaderType::Fragment, fragmentSrc}});
}

Shader::Shader(const std::string &shaderName, const std::string &renderer, const std::unordered_map<ShaderType, std::string> &sources) : ::owl::renderer::Shader{shaderName, renderer} {
	OWL_PROFILE_FUNCTION()

	createShader(sources);
}

Shader::Shader(const std::string &shaderName, const std::string &renderer, const std::vector<std::filesystem::path> &sources) : ::owl::renderer::Shader{shaderName, renderer} {
	OWL_PROFILE_FUNCTION()
	std::unordered_map<ShaderType, std::string> strSources;

	for (const auto &src: sources) {
		ShaderType type = ShaderType::None;
		if (src.extension() == ".frag")
			type = ShaderType::Fragment;
		if (src.extension() == ".vert")
			type = ShaderType::Vertex;
		if (type == ShaderType::None) {
			OWL_CORE_ASSERT(false, "Unknown Shader Type")
			continue;
		}
		strSources.emplace(type, core::utils::fileToString(src));
	}
	createShader(strSources);
}

Shader::~Shader() {
	if (pipelineId > 0) {
		auto &vkh = internal::VulkanHandler::get();
		vkh.popPipeline(pipelineId);
	}
}

void Shader::bind() const {}

void Shader::unbind() const {}

void Shader::setInt(const std::string &, int) {}

void Shader::setIntArray(const std::string &, int *, uint32_t) {}

void Shader::setFloat(const std::string &, float) {}

void Shader::setFloat2(const std::string &, const glm::vec2 &) {}

void Shader::setFloat3(const std::string &, const glm::vec3 &) {}

void Shader::setFloat4(const std::string &, const glm::vec4 &) {}

void Shader::setMat4(const std::string &, const glm::mat4 &) {}

void Shader::createShader(const std::unordered_map<ShaderType, std::string> &) {
}

}// namespace owl::renderer::vulkan
