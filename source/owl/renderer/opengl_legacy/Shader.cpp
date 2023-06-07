/**
 * @file Shader.cpp
 * @author Silmaen
 * @date 30/06/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Shader.h"
#include "core/Application.h"
#include "core/Core.h"

#include "gl_21/glad.h"

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
#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace owl::renderer::opengl_legacy {


Shader::Shader(const std::string &shaderName, [[maybe_unused]] const std::string &vertexSrc, [[maybe_unused]] const std::string &fragmentSrc) : ::owl::renderer::Shader{shaderName} {
}

Shader::Shader(const std::string &shaderName, [[maybe_unused]] const std::unordered_map<ShaderType, std::string> &sources) : ::owl::renderer::Shader{shaderName} {
}

Shader::Shader(const std::string &shaderName, [[maybe_unused]] const std::vector<std::filesystem::path> &sources) : ::owl::renderer::Shader{shaderName} {
																															OWL_PROFILE_FUNCTION()}

																													Shader::~Shader() {
	OWL_PROFILE_FUNCTION()
}

void Shader::bind() const {
	OWL_PROFILE_FUNCTION()

	gl_21::glUseProgram(programID);
}

void Shader::unbind() const {
	OWL_PROFILE_FUNCTION()

	gl_21::glUseProgram(0);
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

void Shader::uploadUniformInt(const std::string &name, int data) const {
	gl_21::GLint location = gl_21::glGetUniformLocation(programID, name.c_str());
	gl_21::glUniform1i(location, data);
}

void Shader::uploadUniformIntArray(const std::string &name, int *values, uint32_t count) const {
	gl_21::GLint location = gl_21::glGetUniformLocation(programID, name.c_str());
	gl_21::glUniform1iv(location, static_cast<gl_21::GLsizei>(count), values);
}

void Shader::uploadUniformFloat(const std::string &name, float value) const {
	gl_21::GLint location = gl_21::glGetUniformLocation(programID, name.c_str());
	gl_21::glUniform1f(location, value);
}

void Shader::uploadUniformFloat2(const std::string &name, const glm::vec2 &value) const {
	gl_21::GLint location = gl_21::glGetUniformLocation(programID, name.c_str());
	gl_21::glUniform2f(location, value.x, value.y);
}

void Shader::uploadUniformFloat3(const std::string &name, const glm::vec3 &value) const {
	gl_21::GLint location = gl_21::glGetUniformLocation(programID, name.c_str());
	gl_21::glUniform3f(location, value.x, value.y, value.z);
}

void Shader::uploadUniformFloat4(const std::string &name, const glm::vec4 &value) const {
	gl_21::GLint location = gl_21::glGetUniformLocation(programID, name.c_str());
	gl_21::glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::uploadUniformMat3(const std::string &name, const glm::mat3 &matrix) const {
	gl_21::GLint location = gl_21::glGetUniformLocation(programID, name.c_str());
	gl_21::glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::uploadUniformMat4(const std::string &name, const glm::mat4 &matrix) const {
	gl_21::GLint location = gl_21::glGetUniformLocation(programID, name.c_str());
	gl_21::glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

}// namespace owl::renderer::opengl_legacy
