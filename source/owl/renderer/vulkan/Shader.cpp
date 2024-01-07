/**
 * @file Shader.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Shader.h"

namespace owl::renderer::vulkan {

Shader::Shader(const std::string &shaderName, const std::string &, const std::string &) : ::owl::renderer::Shader{shaderName} {}

Shader::Shader(const std::string &shaderName, const std::unordered_map<ShaderType, std::string> &) : ::owl::renderer::Shader{shaderName} {}

Shader::Shader(const std::string &shaderName, const std::vector<std::filesystem::path> &) : ::owl::renderer::Shader{shaderName} {}

Shader::~Shader() = default;

void Shader::bind() const {}

void Shader::unbind() const {}

void Shader::setInt(const std::string &, int) {}

void Shader::setIntArray(const std::string &, int *, uint32_t) {}

void Shader::setFloat(const std::string &, float) {}

void Shader::setFloat2(const std::string &, const glm::vec2 &) {}

void Shader::setFloat3(const std::string &, const glm::vec3 &) {}

void Shader::setFloat4(const std::string &, const glm::vec4 &) {}

void Shader::setMat4(const std::string &, const glm::mat4 &) {}


}// namespace owl::renderer::vulkan
