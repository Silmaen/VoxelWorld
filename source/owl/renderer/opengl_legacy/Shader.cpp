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

static const std::vector<std::string> g_texUniforms = {
		"u_Texture0",  "u_Texture1",  "u_Texture2",  "u_Texture3",  "u_Texture4",  "u_Texture5",  "u_Texture6",
		"u_Texture7",  "u_Texture8",  "u_Texture9",  "u_Texture10", "u_Texture11", "u_Texture12", "u_Texture13",
		"u_Texture14", "u_Texture15", "u_Texture16", "u_Texture17", "u_Texture18", "u_Texture19", "u_Texture20",
		"u_Texture21", "u_Texture22", "u_Texture23", "u_Texture24", "u_Texture25", "u_Texture26", "u_Texture27",
		"u_Texture28", "u_Texture29", "u_Texture30", "u_Texture31",
};

Shader::Shader(const std::string &iShaderName, const std::string &iRenderer, const std::string &iVertexSrc,
			   const std::string &iFragmentSrc)
	: ::owl::renderer::Shader{iShaderName, iRenderer} {
	OWL_PROFILE_FUNCTION()

	createShader({{ShaderType::Vertex, iVertexSrc}, {ShaderType::Fragment, iFragmentSrc}});
}

Shader::Shader(const std::string &iShaderName, const std::string &iRenderer,
			   const std::unordered_map<ShaderType, std::string> &iSources)
	: renderer::Shader{iShaderName, iRenderer} {
	OWL_PROFILE_FUNCTION()

	createShader(iSources);
}

Shader::Shader(const std::string &iShaderName, const std::string &iRenderer,
			   const std::vector<std::filesystem::path> &iSources)
	: renderer::Shader{iShaderName, iRenderer} {
	OWL_PROFILE_FUNCTION()
	std::unordered_map<ShaderType, std::string> strSources;

	for (const auto &src: iSources) {
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
	OWL_PROFILE_FUNCTION()

	glDeleteProgram(programId);
}

void Shader::bind() const {
	OWL_PROFILE_FUNCTION()

	glUseProgram(programId);

	// upload uniforms
	if (const auto *ub = UniformBindingLibrary::get().getUniformBuffer(0); ub != nullptr) {
		glm::mat4 uViewProjection;
		memcpy(&uViewProjection, ub->getData(), sizeof(glm::mat4));
		uploadMat4("u_ViewProjection", uViewProjection);
	}
	for (uint32_t texSlotIndex = 0; texSlotIndex < RenderCommand::getMaxTextureSlots(); texSlotIndex++)
		uploadInt(g_texUniforms[texSlotIndex], static_cast<int32_t>(texSlotIndex));
}

void Shader::unbind() const {
	OWL_PROFILE_FUNCTION()

	glUseProgram(0);
}

void Shader::setInt(const std::string &iName, const int iValue) { uploadInt(iName, iValue); }

void Shader::uploadInt(const std::string &iName, const int32_t iValue) const {
	const GLint location = glGetUniformLocation(programId, iName.c_str());
	glUniform1i(location, iValue);
}

void Shader::setIntArray(const std::string &iName, int *iValues, const uint32_t iCount) {
	const GLint location = glGetUniformLocation(programId, iName.c_str());
	glUniform1iv(location, static_cast<GLsizei>(iCount), iValues);
}

void Shader::setFloat(const std::string &iName, const float iValue) {
	const GLint location = glGetUniformLocation(programId, iName.c_str());
	glUniform1f(location, iValue);
}

void Shader::setFloat2(const std::string &iName, const glm::vec2 &iValue) {
	const GLint location = glGetUniformLocation(programId, iName.c_str());
	glUniform2f(location, iValue.x, iValue.y);
}

void Shader::setFloat3(const std::string &iName, const glm::vec3 &iValue) {
	const GLint location = glGetUniformLocation(programId, iName.c_str());
	glUniform3f(location, iValue.x, iValue.y, iValue.z);
}

void Shader::setFloat4(const std::string &iName, const glm::vec4 &iValue) {
	const GLint location = glGetUniformLocation(programId, iName.c_str());
	glUniform4f(location, iValue.x, iValue.y, iValue.z, iValue.w);
}

void Shader::setMat4(const std::string &iName, const glm::mat4 &iValue) { uploadMat4(iName, iValue); }

void Shader::uploadMat4(const std::string &iName, const glm::mat4 &iValue) const {
	const GLint location = glGetUniformLocation(programId, iName.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(iValue));
}

void Shader::createShader(const std::unordered_map<ShaderType, std::string> &iSources) {
	OWL_PROFILE_FUNCTION()

	const auto start = std::chrono::steady_clock::now();

	// vertex shader
	const GLuint vtxShader = glCreateShader(GL_VERTEX_SHADER);
	{
		if (!iSources.contains(ShaderType::Vertex)) {
			OWL_CORE_ERROR("Shader {} does not contains Vertex shader.", getName())
			return;
		}
		const auto *const src = iSources.at(ShaderType::Vertex).c_str();
		glShaderSource(vtxShader, 1, &src, nullptr);
		glCompileShader(vtxShader);
		// Check Compilation
		GLint vertexCompiled = 0;
		glGetShaderiv(vtxShader, GL_COMPILE_STATUS, &vertexCompiled);
		if (vertexCompiled != GL_TRUE) {
			GLsizei logLength = 0;
			GLchar message[1024];
			glGetShaderInfoLog(vtxShader, 1024, &logLength, message);
			OWL_CORE_ERROR("Compilation of Vertex shader ({}) Failed:\n {}", getName(), message)
			glDeleteShader(vtxShader);
			OWL_CORE_ASSERT(true, "")
			return;
		}
	}
	// fragment shader
	const GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	{
		if (!iSources.contains(ShaderType::Fragment)) {
			glDeleteShader(vtxShader);
			OWL_CORE_ERROR("Shader {} does not contains Fragment shader.", getName())
			return;
		}
		const auto *const src = iSources.at(ShaderType::Fragment).c_str();
		glShaderSource(fragShader, 1, &src, nullptr);
		glCompileShader(fragShader);
		// Check Compilation
		GLint fragmentCompiled = 0;
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragmentCompiled);
		if (fragmentCompiled != GL_TRUE) {
			GLsizei logLength = 0;
			GLchar message[1024];
			glGetShaderInfoLog(fragShader, 1024, &logLength, message);
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
		GLint programLinked = 0;
		glGetProgramiv(programId, GL_LINK_STATUS, &programLinked);
		if (programLinked != GL_TRUE) {
			GLsizei logLength = 0;
			GLchar message[1024];
			glGetProgramInfoLog(programId, 1024, &logLength, message);

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
	const auto timer = std::chrono::steady_clock::now() - start;
	double duration =
			static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(timer).count()) / 1000.0;
	OWL_CORE_INFO("Compilation of shader {} in {} ms", getName(), duration)
}

}// namespace owl::renderer::opengl_legacy
