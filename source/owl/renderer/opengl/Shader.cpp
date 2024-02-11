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
#include "renderer/utils/shaderFileUtils.h"

OWL_DIAG_PUSH
OWL_DIAG_DISABLE_CLANG("-Wdocumentation")
#include <glm/gtc/type_ptr.hpp>
OWL_DIAG_POP

#include "core/external/shaderc.h"

namespace owl::renderer::opengl {

namespace utils {

static uint32_t shaderStageToGLShader(const ShaderType &stage) {
	switch (stage) {
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;
		case ShaderType::Geometry:
			return GL_GEOMETRY_SHADER;
		case ShaderType::Compute:
			return GL_COMPUTE_SHADER;
		case ShaderType::None:
			break;
	}
	OWL_CORE_ASSERT(false, "Unsupported Shader Type")
	return 0;
}

}// namespace utils

Shader::Shader(const std::string &shaderName, const std::string &renderer, const std::string &vertexSrc, const std::string &fragmentSrc) : ::owl::renderer::Shader{shaderName, renderer} {
	compile({{ShaderType::Vertex, vertexSrc}, {ShaderType::Fragment, fragmentSrc}});
}

Shader::Shader(const std::string &shaderName, const std::string &renderer, const std::unordered_map<ShaderType, std::string> &sources) : ::owl::renderer::Shader{shaderName, renderer} {
	compile(sources);
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
	compile(strSources);
}

Shader::~Shader() {
	OWL_PROFILE_FUNCTION()

	glDeleteProgram(programID);
}

void Shader::compile(const std::unordered_map<ShaderType, std::string> &sources) {
	OWL_SCOPE_UNTRACK
	OWL_PROFILE_FUNCTION()

	const auto start = std::chrono::steady_clock::now();

	// todo: direct check for openGL binaries...
	renderer::utils::createCacheDirectoryIfNeeded(getRenderer(), "opengl_vulkan");
	compileOrGetVulkanBinaries(sources);
	renderer::utils::createCacheDirectoryIfNeeded(getRenderer(), "opengl");
	compileOrGetOpenGLBinaries();
	createProgram();

	const auto timer = std::chrono::steady_clock::now() - start;
	double duration = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(timer).count()) / 1000.0;
	OWL_CORE_INFO("Compilation of shader {} in {} ms", getName(), duration)
}

void Shader::compileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string> &sources) {
	OWL_PROFILE_FUNCTION()

	shaderc::CompileOptions options;
	options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
	options.SetOptimizationLevel(shaderc_optimization_level_performance);

	std::filesystem::path cacheDirectory = renderer::utils::getCacheDirectory(getRenderer(), "opengl_vulkan");

	auto &shaderData = vulkanSPIRV;
	shaderData.clear();
	for (auto &&[stage, source]: sources) {
		std::filesystem::path basePath = renderer::utils::getShaderPath(getName(), getRenderer(), "opengl", stage);
		std::filesystem::path cachedPath = renderer::utils::getShaderCachedPath(getName(), getRenderer(), "opengl_vulkan", stage);

		if (exists(cachedPath) && (last_write_time(cachedPath) > last_write_time(basePath))) {
			// Cache exists: read it
			OWL_CORE_INFO("Using cached Vulkan Shader {}-{}", getName(), magic_enum::enum_name(stage))
			shaderData[stage] = renderer::utils::readCachedShader(cachedPath);
		} else {
			if (exists(cachedPath))
				OWL_CORE_INFO("Origin file newer than cached one, Recompiling.")
			shaderc::Compiler compiler;
			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source,
																			 renderer::utils::shaderStageToShaderC(stage),
																			 renderer::utils::getShaderPath(getName(), getRenderer(), "opengl", stage).string().c_str(),
																			 options);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				OWL_CORE_ERROR(module.GetErrorMessage())
				OWL_CORE_ASSERT(false, "Failed Compilation")
			}
			shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
			renderer::utils::writeCachedShader(cachedPath, shaderData[stage]);
		}
	}
	for (auto &&[stage, data]: shaderData)
		renderer::utils::shaderReflect(getName(), getRenderer(), "opengl", stage, data);
}

void Shader::compileOrGetOpenGLBinaries() {
	OWL_PROFILE_FUNCTION()

	auto &shaderData = openGLSPIRV;

	shaderc::CompileOptions options;
	options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
	options.SetOptimizationLevel(shaderc_optimization_level_performance);

	std::filesystem::path cacheDirectory = renderer::utils::getCacheDirectory(getRenderer(), "opengl");
	shaderData.clear();
	openGLSource.clear();
	for (auto &&[stage, spirv]: vulkanSPIRV) {
		std::filesystem::path basePath = renderer::utils::getShaderCachedPath(getName(), getRenderer(), "opengl_vulkan", stage);
		std::filesystem::path cachedPath = renderer::utils::getShaderCachedPath(getName(), getRenderer(), "opengl", stage);
		if (exists(cachedPath) && (last_write_time(cachedPath) > last_write_time(basePath))) {
			OWL_CORE_INFO("Using cached OpenGL Shader {}-{}", getName(), magic_enum::enum_name(stage))
			shaderData[stage] = renderer::utils::readCachedShader(cachedPath);
		} else {
			if (exists(cachedPath))
				OWL_CORE_INFO("Origin file newer than cached one, Recompiling.")
			spirv_cross::CompilerGLSL glslCompiler(spirv);
			openGLSource[stage] = glslCompiler.compile();
			auto &source = openGLSource[stage];
			shaderc::Compiler compiler;
			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source,
																			 renderer::utils::shaderStageToShaderC(stage),
																			 renderer::utils::getShaderPath(getName(), getRenderer(), "opengl", stage).string().c_str());
			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				OWL_CORE_ERROR(module.GetErrorMessage())
				OWL_CORE_ASSERT(false, "Compilation error")
			}
			shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
			renderer::utils::writeCachedShader(cachedPath, shaderData[stage]);
		}
	}
}

void Shader::createProgram() {
	const GLuint program = glCreateProgram();

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
		for (const auto id: shaderIDs)
			glDeleteShader(id);
		OWL_CORE_ASSERT(false, fmt::format("Failed to create shader {}", getName()))
		return;
	}
	for (const auto id: shaderIDs) {
		glDetachShader(program, id);
		glDeleteShader(id);
	}
	programID = program;
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
	const GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform1i(location, data);
}

void Shader::uploadUniformIntArray(const std::string &name, int *values, uint32_t count) {
	const GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform1iv(location, static_cast<GLsizei>(count), values);
}

void Shader::uploadUniformFloat(const std::string &name, float value) {
	const GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform1f(location, value);
}

void Shader::uploadUniformFloat2(const std::string &name, const glm::vec2 &value) {
	const GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform2f(location, value.x, value.y);
}

void Shader::uploadUniformFloat3(const std::string &name, const glm::vec3 &value) {
	const GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::uploadUniformFloat4(const std::string &name, const glm::vec4 &value) {
	const GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::uploadUniformMat3(const std::string &name, const glm::mat3 &matrix) {
	const GLint location = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::uploadUniformMat4(const std::string &name, const glm::mat4 &matrix) {
	const GLint location = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

}// namespace owl::renderer::opengl
