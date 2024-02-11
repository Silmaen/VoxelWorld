/**
 * @file shaderFileUtils.h
 * @author Silmaen
 * @date 06/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/external/shaderc.h"
#include "renderer/Shader.h"

namespace owl::renderer::utils {

std::filesystem::path getCacheDirectory(const std::string &renderer, const std::string &rendererAPI);

void createCacheDirectoryIfNeeded(const std::string &renderer, const std::string &rendererAPI);

std::filesystem::path getShaderCachedPath(const std::string &shaderName, const std::string &renderer, const std::string &rendererAPI, const ShaderType &type);

std::filesystem::path getShaderPath(const std::string &shaderName, const std::string &renderer, const std::string &rendererAPI, const ShaderType &type);

std::filesystem::path getRelativeShaderPath(const std::string &shaderName, const std::string &renderer, const std::string &rendererAPI, const ShaderType &type);

std::string getExtension(const ShaderType &stage);

std::string getCacheExtension(const ShaderType &stage);

shaderc_shader_kind shaderStageToShaderC(const ShaderType &stage);

std::vector<uint32_t> readCachedShader(const std::filesystem::path &file);

bool writeCachedShader(const std::filesystem::path &file, const std::vector<uint32_t> &data);

void shaderReflect(const std::string &shaderName, const std::string &renderer, const std::string &rendererAPI, ShaderType stage, const std::vector<uint32_t> &shaderData);

}// namespace owl::renderer::utils
