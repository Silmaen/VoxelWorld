/**
 * @file Shader.cpp
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Shader.h"
#include "core/Core.h"

#include <glad/glad.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif
#include <glm/gtc/type_ptr.hpp>
#include <magic_enum.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace owl::renderer::opengl {

Shader::Shader(const std::string &shaderName, const std::string &vertexSrc, const std::string &fragmentSrc) : ::owl::renderer::Shader{shaderName} {
	compile({{ShaderType::Vertex, vertexSrc}, {ShaderType::Fragment, fragmentSrc}});
}

Shader::Shader(const std::string &shaderName, const std::unordered_map<ShaderType, std::string> &sources): ::owl::renderer::Shader{shaderName}  {
	compile(sources);
}

inline static std::string fileToString(const std::filesystem::path &file) {
	if (!exists(file)) {
		OWL_CORE_WARN("Shader file '{}' does not exists", file.string())
		return "";
	}
	std::ifstream t(file);
	std::string str;
	t.seekg(0, std::ios::end);
	str.reserve(static_cast<size_t>(t.tellg()));
	t.seekg(0, std::ios::beg);
	str.assign((std::istreambuf_iterator<char>(t)),
			   std::istreambuf_iterator<char>());
	return str;
}

Shader::Shader(const std::string &shaderName, const std::vector<std::filesystem::path> &sources): ::owl::renderer::Shader{shaderName}  {
	OWL_PROFILE_FUNCTION()

	std::unordered_map<ShaderType, std::string> strSources;
	for(const auto& src: sources){
		ShaderType type = ShaderType::None;
		if (src.extension() == ".frag")
			type = ShaderType::Fragment;
		if (src.extension() == ".vert")
			type = ShaderType::Vertex;
		if (type == ShaderType::None){
			OWL_CORE_ASSERT(false, "Unknown Shader Type")
			continue;
		}
		strSources.emplace(type, fileToString(src));
	}
	compile(strSources);
}

void Shader::compile(const std::unordered_map<ShaderType, std::string> &sources) {
	OWL_PROFILE_FUNCTION()

	auto start = std::chrono::steady_clock::now();

	// Get a program object.
	GLuint program = glCreateProgram();

	// list of shader's id
	std::vector<GLuint> shaderIDs;

	bool AllOk = true;

	for (const auto &source: sources) {
		// Create an empty vertex shader handle
		GLuint shaderID = 0;
		switch (source.first) {
			case ShaderType::Fragment:
				shaderID = glCreateShader(GL_FRAGMENT_SHADER);
				break;
			case ShaderType::Vertex:
				shaderID = glCreateShader(GL_VERTEX_SHADER);
				break;
			case ShaderType::None:
				OWL_CORE_ASSERT(true, "Unknown Shader Type")
				AllOk = false;
				continue;
		}
		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar *src = source.second.c_str();
		glShaderSource(shaderID, 1, &src, nullptr);

		// Compile the vertex shader
		glCompileShader(shaderID);

		GLint isCompiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(static_cast<size_t>(maxLength));
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(shaderID);

			OWL_CORE_ERROR("{0}", infoLog.data())
			OWL_CORE_TRACE("Shader read: {}", shaderID)
			OWL_CORE_ASSERT(false, "Shader compilation failure!")
			AllOk = false;
		} else {
			shaderIDs.push_back(shaderID);
		}
	}
	if (!AllOk) {
		for (auto sId: shaderIDs)
			glDeleteShader(sId);
		glDeleteProgram(program);
		return;
	}
	// all sources compiled successfully, now link
	// Attach our shaders to our program
	for (auto sId: shaderIDs)
		glAttachShader(program, sId);
	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, static_cast<int *>(&isLinked));
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(static_cast<size_t>(maxLength));
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		for (auto sId: shaderIDs)
			glDeleteShader(sId);

		OWL_CORE_ERROR("{}", infoLog.data())
		OWL_CORE_ASSERT(false, "Shader link failure!")
		return;
	}
	// Always detach shaders after a successful link.
	for (auto sId: shaderIDs) {
		glDetachShader(program, sId);
		glDeleteShader(sId);
	}
	programID = program;

	auto timer = std::chrono::steady_clock::now() - start;
	double duration = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(timer).count()) / 1000.0;
	OWL_CORE_INFO("Compilation of shader {} in {} ms",getName(), duration)
}

Shader::~Shader() {
	OWL_PROFILE_FUNCTION()

	glDeleteProgram(programID);
}
;
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
void Shader::setIntArray(const std::string& name, int* values, uint32_t count) {
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
void Shader::uploadUniformIntArray(const std::string& name, int* values, uint32_t count){
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform1iv(location, count, values);
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
