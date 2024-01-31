/**
 * @file Shader.cpp
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Shader.h"
#include "UniformBindingLibrary.h"
#include "core/external/opengl21.h"
#include "core/utils/FileUtils.h"
#include "renderer/RenderCommand.h"
OWL_DIAG_PUSH
OWL_DIAG_DISABLE_CLANG("-Wdocumentation")
#include <glm/gtc/type_ptr.hpp>
OWL_DIAG_POP

namespace owl::renderer::opengl_legacy {

static const std::vector<std::string> texUniforms = {
		"u_Texture0",
		"u_Texture1",
		"u_Texture2",
		"u_Texture3",
		"u_Texture4",
		"u_Texture5",
		"u_Texture6",
		"u_Texture7",
		"u_Texture8",
		"u_Texture9",
		"u_Texture10",
		"u_Texture11",
		"u_Texture12",
		"u_Texture13",
		"u_Texture14",
		"u_Texture15",
		"u_Texture16",
		"u_Texture17",
		"u_Texture18",
		"u_Texture19",
		"u_Texture20",
		"u_Texture21",
		"u_Texture22",
		"u_Texture23",
		"u_Texture24",
		"u_Texture25",
		"u_Texture26",
		"u_Texture27",
		"u_Texture28",
		"u_Texture29",
		"u_Texture30",
		"u_Texture31",
};

Shader::Shader(const std::string &shaderName, const std::string &vertexSrc, const std::string &fragmentSrc) : ::owl::renderer::Shader{shaderName} {
	OWL_PROFILE_FUNCTION()

	createShader({{ShaderType::Vertex, vertexSrc}, {ShaderType::Fragment, fragmentSrc}});
}

Shader::Shader(const std::string &shaderName, const std::unordered_map<ShaderType, std::string> &sources) : ::owl::renderer::Shader{shaderName} {
	OWL_PROFILE_FUNCTION()

	createShader(sources);
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
	createShader(strSources);
}

Shader::~Shader() {
	OWL_PROFILE_FUNCTION()

	glDeleteProgram(programId);
}

void Shader::bind() const {
	OWL_PROFILE_FUNCTION()

	glUseProgram(programId);

	// upload uniforms
	auto ubl = UniformBindingLibrary::get();
	auto ub = ubl.getUniformBuffer(0);
	if (ub != nullptr) {
		glm::mat4 u_ViewProjection;
		memcpy(&u_ViewProjection, ub->getData(), sizeof(glm::mat4));
		uploadMat4("u_ViewProjection", u_ViewProjection);
	}
	for (uint32_t texSlotIndex = 0; texSlotIndex < RenderCommand::getMaxTextureSlots(); texSlotIndex++)
		uploadInt(texUniforms[texSlotIndex], static_cast<int32_t>(texSlotIndex));
}

void Shader::unbind() const {
	OWL_PROFILE_FUNCTION()

	glUseProgram(0);
}

void Shader::setInt(const std::string &name, int data) {
	uploadInt(name, data);
}

void Shader::uploadInt(const std::string &name, int32_t data) const {
	GLint location = glGetUniformLocation(programId, name.c_str());
	glUniform1i(location, data);
}

void Shader::setIntArray(const std::string &name, int *values, uint32_t count) {
	GLint location = glGetUniformLocation(programId, name.c_str());
	glUniform1iv(location, static_cast<GLsizei>(count), values);
}

void Shader::setFloat(const std::string &name, float value) {
	GLint location = glGetUniformLocation(programId, name.c_str());
	glUniform1f(location, value);
}

void Shader::setFloat2(const std::string &name, const glm::vec2 &value) {
	GLint location = glGetUniformLocation(programId, name.c_str());
	glUniform2f(location, value.x, value.y);
}

void Shader::setFloat3(const std::string &name, const glm::vec3 &value) {
	GLint location = glGetUniformLocation(programId, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::setFloat4(const std::string &name, const glm::vec4 &value) {
	GLint location = glGetUniformLocation(programId, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &matrix) {
	uploadMat4(name, matrix);
}

void Shader::uploadMat4(const std::string &name, const glm::mat4 &matrix) const {
	GLint location = glGetUniformLocation(programId, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::createShader(const std::unordered_map<ShaderType, std::string> &sources) {
	OWL_PROFILE_FUNCTION()

	auto start = std::chrono::steady_clock::now();

	// vertex shader
	GLuint vtxShader = glCreateShader(GL_VERTEX_SHADER);
	{
		if (!sources.contains(ShaderType::Vertex)) {
			OWL_CORE_ERROR("Shader {} does not contains Vertex shader.", getName())
			return;
		}
		auto src = sources.at(ShaderType::Vertex).c_str();
		glShaderSource(vtxShader, 1, &src, nullptr);
		glCompileShader(vtxShader);
		// Check Compilation
		GLint vertex_compiled;
		glGetShaderiv(vtxShader, GL_COMPILE_STATUS, &vertex_compiled);
		if (vertex_compiled != GL_TRUE) {
			GLsizei log_length = 0;
			GLchar message[1024];
			glGetShaderInfoLog(vtxShader, 1024, &log_length, message);
			OWL_CORE_ERROR("Compilation of Vertex shader ({}) Failed:\n {}", getName(), message)
			glDeleteShader(vtxShader);
			OWL_CORE_ASSERT(true, "")
			return;
		}
	}
	// fragment shader
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	{
		if (!sources.contains(ShaderType::Fragment)) {
			glDeleteShader(vtxShader);
			OWL_CORE_ERROR("Shader {} does not contains Fragment shader.", getName())
			return;
		}
		auto src = sources.at(ShaderType::Fragment).c_str();
		glShaderSource(fragShader, 1, &src, nullptr);
		glCompileShader(fragShader);
		// Check Compilation
		GLint fragment_compiled;
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragment_compiled);
		if (fragment_compiled != GL_TRUE) {
			GLsizei log_length = 0;
			GLchar message[1024];
			glGetShaderInfoLog(fragShader, 1024, &log_length, message);
			OWL_CORE_ERROR("Compilation of Fragment shader ({}) Failed:\n {}", getName(), message)
			glDeleteShader(vtxShader);
			glDeleteShader(fragShader);
			OWL_CORE_ASSERT(true, "")
			return;
		}
	}
	// Program
	programId = glCreateProgram();
	{
		glAttachShader(programId, vtxShader);
		glAttachShader(programId, fragShader);
		glLinkProgram(programId);
		GLint program_linked;
		glGetProgramiv(programId, GL_LINK_STATUS, &program_linked);
		if (program_linked != GL_TRUE) {
			GLsizei log_length = 0;
			GLchar message[1024];
			glGetProgramInfoLog(programId, 1024, &log_length, message);

			OWL_CORE_ERROR("Link of shader ({}) Failed:\n {}", getName(), message)
			glDetachShader(programId, vtxShader);
			glDeleteShader(vtxShader);
			glDetachShader(programId, fragShader);
			glDeleteShader(fragShader);
			glDeleteProgram(programId);
			programId = 0;
			return;
		}
	}
	// print timing
	auto timer = std::chrono::steady_clock::now() - start;
	double duration = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(timer).count()) / 1000.0;
	OWL_CORE_INFO("Compilation of shader {} in {} ms", getName(), duration)
}

}// namespace owl::renderer::opengl_legacy
