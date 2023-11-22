/**
 * @file Shader.cpp
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Shader.h"
#include "core/Application.h"
#include "core/Core.h"
#include "core/external/opengl46.h"
#include "core/utils/FileUtils.h"
#include "debug/Tracker.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif
#include <glm/gtc/type_ptr.hpp>
#include <magic_enum.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif
#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace owl::renderer::opengl {

namespace utils {

static std::filesystem::path getCacheDirectory() {
	return core::Application::get().getAssetDirectory() / "cache" / "shader" / "opengl";
}

static void createCacheDirectoryIfNeeded() {
	std::filesystem::path cacheDirectory = getCacheDirectory();
	if (!std::filesystem::exists(cacheDirectory))
		std::filesystem::create_directories(cacheDirectory);
}

enum struct CacheType {
	OpenGL,
	Vulkan
};

static shaderc_shader_kind shaderStageToShaderC(const ShaderType &stage) {
	switch (stage) {
		case ShaderType::Vertex:
			return shaderc_glsl_vertex_shader;
		case ShaderType::Fragment:
			return shaderc_glsl_fragment_shader;
		case ShaderType::None:
			break;
	}
	OWL_CORE_ASSERT(false, "Unsupported Shader Type")
	return static_cast<shaderc_shader_kind>(0);
}

static uint32_t shaderStageToGLShader(const ShaderType &stage) {
	switch (stage) {
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;
		case ShaderType::None:
			break;
	}
	OWL_CORE_ASSERT(false, "Unsupported Shader Type")
	return 0;
}

static std::string getExtension(const ShaderType &stage) {
	auto ext = fmt::format(".{}", magic_enum::enum_name(stage).substr(0, 4));
	std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });
	return ext;
}

static std::string getCacheExtension(const CacheType &type, const ShaderType &stage) {
	auto ext = fmt::format(".cached_{}.{}", magic_enum::enum_name(type), magic_enum::enum_name(stage).substr(0, 4));
	std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });
	return ext;
}

static std::vector<uint32_t> readCachedShader(const std::filesystem::path &file) {
	OWL_PROFILE_FUNCTION()

	std::vector<uint32_t> result;
	std::ifstream in(file, std::ios::in | std::ios::binary);
	in.seekg(0, std::ios::end);
	auto size = in.tellg();
	in.seekg(0, std::ios::beg);
	result.resize(static_cast<size_t>(size) / sizeof(uint32_t));
	in.read(reinterpret_cast<char *>(result.data()), size);
	in.close();
	return result;
}

static bool writeCachedShader(const std::filesystem::path &file, const std::vector<uint32_t> &data) {
	OWL_PROFILE_FUNCTION()

	std::ofstream out(file, std::ios::out | std::ios::binary);
	if (out.is_open()) {
		out.write(reinterpret_cast<const char *>(data.data()), static_cast<long long>(data.size() * sizeof(uint32_t)));
		out.flush();
		out.close();
		return true;
	}
	return false;
}

static std::filesystem::path getShaderCachedPath(const std::string &shaderName, const utils::CacheType &cache, const ShaderType &type) {
	return utils::getCacheDirectory() / (shaderName + utils::getCacheExtension(cache, type));
}
static std::filesystem::path getShaderPath(const std::string &shaderName, const ShaderType &type) {
	return core::Application::get().getAssetDirectory() / "shaders" / (shaderName + getExtension(type));
}

}// namespace utils

Shader::Shader(const std::string &shaderName, const std::string &vertexSrc, const std::string &fragmentSrc) : ::owl::renderer::Shader{shaderName} {
	compile({{ShaderType::Vertex, vertexSrc}, {ShaderType::Fragment, fragmentSrc}});
}

Shader::Shader(const std::string &shaderName, const std::unordered_map<ShaderType, std::string> &sources) : ::owl::renderer::Shader{shaderName} {
	compile(sources);
}

Shader::Shader(const std::string &shaderName, const std::vector<std::filesystem::path> &sources) : ::owl::renderer::Shader{shaderName} {
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
	compile(strSources);
}

Shader::~Shader() {
	OWL_PROFILE_FUNCTION()

	glDeleteProgram(programID);
}

void Shader::compile(const std::unordered_map<ShaderType, std::string> &sources) {
	OWL_SCOPE_UNTRACK
	OWL_PROFILE_FUNCTION()

	auto start = std::chrono::steady_clock::now();

	utils::createCacheDirectoryIfNeeded();
	compileOrGetVulkanBinaries(sources);
	compileOrGetOpenGLBinaries();
	createProgram();

	auto timer = std::chrono::steady_clock::now() - start;
	double duration = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(timer).count()) / 1000.0;
	OWL_CORE_INFO("Compilation of shader {} in {} ms", getName(), duration)
}

void Shader::compileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string> &sources) {
	OWL_PROFILE_FUNCTION()

	shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
	options.SetOptimizationLevel(shaderc_optimization_level_performance);

	std::filesystem::path cacheDirectory = utils::getCacheDirectory();

	auto &shaderData = vulkanSPIRV;
	shaderData.clear();
	for (auto &&[stage, source]: sources) {
		std::filesystem::path basePath = utils::getShaderPath(getName(), stage);
		std::filesystem::path cachedPath = utils::getShaderCachedPath(getName(), utils::CacheType::Vulkan, stage);

		if (exists(cachedPath) && (last_write_time(cachedPath) > last_write_time(basePath))) {
			// Cache exists: read it
			OWL_CORE_TRACE("Using cached Vulkan Shader {}-{}", getName(), magic_enum::enum_name(stage))
			shaderData[stage] = utils::readCachedShader(cachedPath);
		} else {
			if (exists(cachedPath))
				OWL_CORE_TRACE("Origin file newer than cached one, Recompiling.")
			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source,
																			 utils::shaderStageToShaderC(stage),
																			 utils::getShaderPath(getName(), stage).string().c_str(),
																			 options);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				OWL_CORE_ERROR(module.GetErrorMessage())
				OWL_CORE_ASSERT(false, "Failed Compilation")
			}
			shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
			utils::writeCachedShader(cachedPath, shaderData[stage]);
		}
	}
	for (auto &&[stage, data]: shaderData)
		reflect(stage, data);
}

void Shader::compileOrGetOpenGLBinaries() {
	OWL_PROFILE_FUNCTION()

	auto &shaderData = openGLSPIRV;

	shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
	options.SetOptimizationLevel(shaderc_optimization_level_performance);

	std::filesystem::path cacheDirectory = utils::getCacheDirectory();
	shaderData.clear();
	openGLSource.clear();
	for (auto &&[stage, spirv]: vulkanSPIRV) {
		std::filesystem::path basePath = utils::getShaderCachedPath(getName(), utils::CacheType::Vulkan, stage);
		std::filesystem::path cachedPath = utils::getShaderCachedPath(getName(), utils::CacheType::OpenGL, stage);
		if (exists(cachedPath) && (last_write_time(cachedPath) > last_write_time(basePath))) {
			OWL_CORE_TRACE("Using cached OpenGL Shader {}-{}", getName(), magic_enum::enum_name(stage))
			shaderData[stage] = utils::readCachedShader(cachedPath);
		} else {
			if (exists(cachedPath))
				OWL_CORE_TRACE("Origin file newer than cached one, Recompiling.")
			spirv_cross::CompilerGLSL glslCompiler(spirv);
			openGLSource[stage] = glslCompiler.compile();
			auto &source = openGLSource[stage];
			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source,
																			 utils::shaderStageToShaderC(stage),
																			 utils::getShaderPath(getName(), stage).string().c_str());
			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				OWL_CORE_ERROR(module.GetErrorMessage())
				OWL_CORE_ASSERT(false, "Compilation error")
			}
			shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
			utils::writeCachedShader(cachedPath, shaderData[stage]);
		}
	}
}

void Shader::createProgram() {
	GLuint program = glCreateProgram();

	// list of shader's id
	std::vector<GLuint> shaderIDs;
	for (auto &&[stage, spirv]: openGLSPIRV) {
		GLuint shaderID = shaderIDs.emplace_back(glCreateShader(utils::shaderStageToGLShader(stage)));
		OWL_CORE_ASSERT(!spirv.empty(), "Empty shader data")
		glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
		glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
		glAttachShader(program, shaderID);
	}
	glLinkProgram(program);
	GLint isLinked;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		OWL_CORE_ERROR("Shader linking failed ({})", getName())
		GLint maxLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		if (maxLength > 0) {
			std::vector<GLchar> infoLog(static_cast<size_t>(maxLength));
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			OWL_CORE_ERROR("     Details: {}", infoLog.data())
		}
		glDeleteProgram(program);
		for (auto id: shaderIDs)
			glDeleteShader(id);
		OWL_CORE_ASSERT(false, fmt::format("Failed to create shader {}", getName()))
		return;
	}
	for (auto id: shaderIDs) {
		glDetachShader(program, id);
		glDeleteShader(id);
	}
	programID = program;
}

void Shader::reflect(ShaderType stage, const std::vector<uint32_t> &shaderData) {
	spirv_cross::Compiler compiler(shaderData);
	spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	OWL_CORE_TRACE("opengl::Shader::reflect - {0} : {1}", magic_enum::enum_name(stage), utils::getShaderPath(getName(), stage).string())
	OWL_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size())
	OWL_CORE_TRACE("    {0} resources", resources.sampled_images.size())

	if (resources.uniform_buffers.empty()) {
		OWL_CORE_TRACE("No Uniform buffer")
	} else {
		OWL_CORE_TRACE("Uniform buffers:")
		for (const auto &resource: resources.uniform_buffers) {
			const auto &bufferType = compiler.get_type(resource.base_type_id);
			OWL_CORE_TRACE("  {}", resource.name)
			OWL_CORE_TRACE("    Size = {}", compiler.get_declared_struct_size(bufferType))
			OWL_CORE_TRACE("    Binding = {}", compiler.get_decoration(resource.id, spv::DecorationBinding))
			OWL_CORE_TRACE("    Members = {}", bufferType.member_types.size())
		}
	}
}

void Shader::bind() const {
	OWL_PROFILE_FUNCTION()

	glUseProgram(programID);
}

void Shader::unbind() const {
	OWL_PROFILE_FUNCTION()

	glUseProgram(0);
}

void Shader::setInt(const std::string &name, int value) {
	OWL_PROFILE_FUNCTION()

	uploadUniformInt(name, value);
}

void Shader::setIntArray(const std::string &name, int *values, uint32_t count) {
	OWL_PROFILE_FUNCTION()

	uploadUniformIntArray(name, values, count);
}

void Shader::setFloat(const std::string &name, float value) {
	OWL_PROFILE_FUNCTION()

	uploadUniformFloat(name, value);
}

void Shader::setFloat2(const std::string &name, const glm::vec2 &value) {
	OWL_PROFILE_FUNCTION()

	uploadUniformFloat2(name, value);
}

void Shader::setFloat3(const std::string &name, const glm::vec3 &value) {
	OWL_PROFILE_FUNCTION()

	uploadUniformFloat3(name, value);
}

void Shader::setFloat4(const std::string &name, const glm::vec4 &value) {
	OWL_PROFILE_FUNCTION()

	uploadUniformFloat4(name, value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &matrix) {
	OWL_PROFILE_FUNCTION()

	uploadUniformMat4(name, matrix);
}

void Shader::uploadUniformInt(const std::string &name, int data) {
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform1i(location, data);
}

void Shader::uploadUniformIntArray(const std::string &name, int *values, uint32_t count) {
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform1iv(location, static_cast<GLsizei>(count), values);
}

void Shader::uploadUniformFloat(const std::string &name, float value) {
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform1f(location, value);
}

void Shader::uploadUniformFloat2(const std::string &name, const glm::vec2 &value) {
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform2f(location, value.x, value.y);
}

void Shader::uploadUniformFloat3(const std::string &name, const glm::vec3 &value) {
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::uploadUniformFloat4(const std::string &name, const glm::vec4 &value) {
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::uploadUniformMat3(const std::string &name, const glm::mat3 &matrix) {
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::uploadUniformMat4(const std::string &name, const glm::mat4 &matrix) {
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

}// namespace owl::renderer::opengl
