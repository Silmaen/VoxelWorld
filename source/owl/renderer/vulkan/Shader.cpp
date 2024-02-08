/**
 * @file Shader.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Shader.h"
#include "core/Application.h"
#include "core/external/shaderc.h"
#include "core/utils/FileUtils.h"
#include "debug/Tracker.h"
#include "internal/VulkanHandler.h"
#include "internal/utils.h"
#include "renderer/utils/shaderFileUtils.h"

namespace owl::renderer::vulkan {

namespace utils {

[[maybe_unused]] static VkShaderStageFlagBits shaderStageToVkStageBit(const ShaderType &stage) {
	switch (stage) {
		case ShaderType::Vertex:
			return VK_SHADER_STAGE_VERTEX_BIT;
		case ShaderType::Fragment:
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		case ShaderType::Geometry:
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		case ShaderType::Compute:
			return VK_SHADER_STAGE_COMPUTE_BIT;
		case ShaderType::None:
			return static_cast<VkShaderStageFlagBits>(0);
	}
	return static_cast<VkShaderStageFlagBits>(0);
}


static VkShaderModule createShaderModule(VkDevice logicalDevice, const std::vector<uint32_t> &code) {
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size() * sizeof(uint32_t);
	createInfo.pCode = code.data();

	VkShaderModule shaderModule;
	const VkResult result = vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS) {
		OWL_CORE_ERROR("failed to create shader module ({}).", internal::resultString(result))
		return nullptr;
	}

	return shaderModule;
}

}// namespace utils

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
		auto type = ShaderType::None;
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
	if (pipelineId < 0)
		return;
	auto &vkh = internal::VulkanHandler::get();
	vkh.popPipeline(pipelineId);
}

void Shader::bind() const {
	if (pipelineId < 0)
		return;
	auto &vkh = internal::VulkanHandler::get();
	vkh.bindPipeline(pipelineId);
}

void Shader::unbind() const {}

void Shader::setInt(const std::string &, int) {}

void Shader::setIntArray(const std::string &, int *, uint32_t) {}

void Shader::setFloat(const std::string &, float) {}

void Shader::setFloat2(const std::string &, const glm::vec2 &) {}

void Shader::setFloat3(const std::string &, const glm::vec3 &) {}

void Shader::setFloat4(const std::string &, const glm::vec4 &) {}

void Shader::setMat4(const std::string &, const glm::mat4 &) {}

void Shader::createShader(const std::unordered_map<ShaderType, std::string> &sources) {
	OWL_SCOPE_UNTRACK

	OWL_PROFILE_FUNCTION()
	const auto start = std::chrono::steady_clock::now();

	renderer::utils::createCacheDirectoryIfNeeded(getRenderer(), "vulkan");
	compileOrGetVulkanBinaries(sources);
	createPipeline();

	const auto timer = std::chrono::steady_clock::now() - start;
	double duration = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(timer).count()) / 1000.0;
	OWL_CORE_INFO("Compilation of shader {} in {} ms", getName(), duration)
}

void Shader::compileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string> &sources) {
	OWL_PROFILE_FUNCTION()

	shaderc::CompileOptions options;
	options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_0);
	options.SetOptimizationLevel(shaderc_optimization_level_performance);

	std::filesystem::path cacheDirectory = renderer::utils::getCacheDirectory(getRenderer(), "vulkan");

	auto &shaderData = vulkanSPIRV;
	shaderData.clear();
	for (auto &&[stage, source]: sources) {
		std::filesystem::path basePath = renderer::utils::getShaderPath(getName(), getRenderer(), "vulkan", stage);
		std::filesystem::path cachedPath = renderer::utils::getShaderCachedPath(getName(), getRenderer(), "vulkan", stage);

		if (exists(cachedPath) && (last_write_time(cachedPath) > last_write_time(basePath))) {
			// Cache exists: read it
			OWL_CORE_INFO("Using cached Vulkan Shader {}-{}", getName(), magic_enum::enum_name(stage))
			shaderData[stage] = renderer::utils::readCachedShader(cachedPath);
		} else {
			if (exists(cachedPath))
				OWL_CORE_INFO("Origin file newer than cached one, Recompiling.")
			OWL_CORE_TRACE("Compile shader file: {}", renderer::utils::getRelativeShaderPath(getName(), getRenderer(), "vulkan", stage).string())
			shaderc::Compiler compiler;
			auto module = compiler.CompileGlslToSpv(source,
													renderer::utils::shaderStageToShaderC(stage),
													renderer::utils::getShaderPath(getName(), getRenderer(), "vulkan", stage).string().c_str(),
													options);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				OWL_CORE_ERROR(module.GetErrorMessage())
				OWL_CORE_ASSERT(false, "Failed Compilation")
			}
			shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
			OWL_CORE_TRACE("Write compiled shader file, size {}", shaderData[stage].size())
			if (!renderer::utils::writeCachedShader(cachedPath, shaderData[stage]))
				OWL_CORE_WARN("Failed to write the compiled shader.")
		}
	}
	for (auto &&[stage, data]: shaderData)
		renderer::utils::shaderReflect(getName(), getRenderer(), "vulkan", stage, data);
}

void Shader::createPipeline() {
	auto &vkh = internal::VulkanHandler::get();
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	for (auto &&[stage, code]: vulkanSPIRV) {
		shaderStages.emplace_back();
		shaderStages.back().sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages.back().stage = utils::shaderStageToVkStageBit(stage);
		shaderStages.back().module = utils::createShaderModule(vkh.getDevice(), code);
		if (shaderStages.back().module == nullptr) {
			OWL_CORE_ERROR("Vulkan: Failed create shader module {} {}", getName(), magic_enum::enum_name(stage))
			vkh.setState(internal::VulkanHandler::State::ErrorCreatingPipeline);
			return;
		}
		shaderStages.back().pName = "main";
	}
	if (pipelineId >= 0)
		vkh.popPipeline(pipelineId);
	pipelineId = vkh.pushPipeline(getName(), shaderStages);
	for (const auto &stage: shaderStages) {
		vkDestroyShaderModule(vkh.getDevice(), stage.module, nullptr);
	}
	if (pipelineId < 0) {
		OWL_CORE_WARN("Vulkan shader: Failed to register shader {}.", getName())
	}
}

}// namespace owl::renderer::vulkan
