/**
 * @file shaderFileUtils.cpp
 * @author Silmaen
 * @date 06/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "shaderFileUtils.h"
#include <core/Application.h>


namespace owl::renderer::utils {

std::filesystem::path getCacheDirectory(const std::string &renderer, const std::string &rendererAPI) {
	auto output = core::Application::get().getAssetDirectory() / "cache" / "shader";
	if (!renderer.empty())
		output /= renderer;
	if (!rendererAPI.empty())
		output /= rendererAPI;
	return output;
}

void createCacheDirectoryIfNeeded(const std::string &renderer, const std::string &rendererAPI) {
	std::filesystem::path cacheDirectory = getCacheDirectory(renderer, rendererAPI);
	if (!exists(cacheDirectory)) {
		create_directories(cacheDirectory);
		if (!exists(cacheDirectory))
			OWL_CORE_ERROR("Cannot Create directory {}.", cacheDirectory.string())
	}
}

std::filesystem::path getShaderCachedPath(const std::string &shaderName, const std::string &renderer, const std::string &rendererAPI, const ShaderType &type) {
	return getCacheDirectory(renderer, rendererAPI) / (shaderName + getCacheExtension(type));
}

std::filesystem::path getShaderPath(const std::string &shaderName, const std::string &renderer, const std::string &rendererAPI, const ShaderType &type) {
	return core::Application::get().getAssetDirectory() / "shaders" / renderer / rendererAPI / (shaderName + getExtension(type));
}

std::filesystem::path getRelativeShaderPath(const std::string &shaderName, const std::string &renderer, const std::string &rendererAPI, const ShaderType &type) {
	return std::filesystem::path("shaders") / renderer / rendererAPI / (shaderName + getExtension(type));
}

std::string getExtension(const ShaderType &stage) {
	auto ext = fmt::format(".{}", magic_enum::enum_name(stage).substr(0, 4));
	std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });
	return ext;
}

std::string getCacheExtension(const ShaderType &stage) {
	auto ext = fmt::format(".{}.spv", magic_enum::enum_name(stage).substr(0, 4));
	std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });
	return ext;
}


std::vector<uint32_t> readCachedShader(const std::filesystem::path &file) {
	OWL_PROFILE_FUNCTION()

	std::vector<uint32_t> result;
	std::ifstream in(file, std::ios::in | std::ios::binary);
	in.seekg(0, std::ios::end);
	const auto size = in.tellg();
	in.seekg(0, std::ios::beg);
	result.resize(static_cast<size_t>(size) / sizeof(uint32_t));
	in.read(reinterpret_cast<char *>(result.data()), size);
	in.close();
	return result;
}

bool writeCachedShader(const std::filesystem::path &file, const std::vector<uint32_t> &data) {
	OWL_PROFILE_FUNCTION()
	std::ofstream out(file, std::ios::out | std::ios::binary);
	if (!exists(file.parent_path())) {
		OWL_CORE_WARN("Cache directory {} does not exists, creating.", file.parent_path().string())
	}
	if (out.is_open()) {
		out.write(reinterpret_cast<const char *>(data.data()), static_cast<long long>(data.size() * sizeof(uint32_t)));
		out.flush();
		out.close();
		return true;
	}
	OWL_CORE_WARN("Cannot open file {} for writting.", file.string())
	return false;
}

shaderc_shader_kind shaderStageToShaderC(const ShaderType &stage) {
	switch (stage) {
		case ShaderType::Vertex:
			return shaderc_glsl_vertex_shader;
		case ShaderType::Fragment:
			return shaderc_glsl_fragment_shader;
		case ShaderType::Geometry:
			return shaderc_glsl_geometry_shader;
		case ShaderType::Compute:
			return shaderc_glsl_compute_shader;
		case ShaderType::None:
			break;
	}
	OWL_CORE_ASSERT(false, "Unsupported Shader Type")
	return static_cast<shaderc_shader_kind>(0);
}

void shaderReflect(const std::string &shaderName, const std::string &renderer, const std::string &rendererAPI, ShaderType stage, const std::vector<uint32_t> &shaderData) {
	const spirv_cross::Compiler compiler(shaderData);
	spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	OWL_CORE_TRACE("Shader reflect - {0} : <assets>/{1}",
				   magic_enum::enum_name(stage),
				   renderer::utils::getRelativeShaderPath(shaderName, renderer, rendererAPI, stage).string())
	OWL_CORE_TRACE("    {} resources", resources.sampled_images.size())
	if (resources.uniform_buffers.empty()) {
		OWL_CORE_TRACE("  No Uniform buffer")
	} else {
		OWL_CORE_TRACE("  Uniform buffers:")
		for (const auto &resource: resources.uniform_buffers) {
			const auto &bufferType = compiler.get_type(resource.base_type_id);
			OWL_CORE_TRACE("   {}", resource.name)
			OWL_CORE_TRACE("     Size = {}", compiler.get_declared_struct_size(bufferType))
			OWL_CORE_TRACE("     Binding = {}", compiler.get_decoration(resource.id, spv::DecorationBinding))
			OWL_CORE_TRACE("     Members = {}", bufferType.member_types.size())
		}
	}
}

}// namespace owl::renderer::utils
