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
std::filesystem::path getCacheDirectory(const std::string& iRenderer, const std::string& iRendererApi);

void createCacheDirectoryIfNeeded(const std::string& iRenderer, const std::string& iRendererApi);

std::filesystem::path getShaderCachedPath(const std::string& iShaderName, const std::string& iRenderer,
                                          const std::string& iRendererApi, const ShaderType& iType);

std::filesystem::path getShaderPath(const std::string& iShaderName, const std::string& iRenderer,
                                    const std::string& iRendererApi, const ShaderType& iType);

std::filesystem::path getRelativeShaderPath(const std::string& iShaderName, const std::string& iRenderer,
                                            const std::string& iRendererApi, const ShaderType& iType);

std::string getExtension(const ShaderType& iStage);

std::string getCacheExtension(const ShaderType& iStage);

shaderc_shader_kind shaderStageToShaderC(const ShaderType& iStage);

std::vector<uint32_t> readCachedShader(const std::filesystem::path& iFile);

bool writeCachedShader(const std::filesystem::path& iFile, const std::vector<uint32_t>& iData);

void shaderReflect(const std::string& iShaderName, const std::string& iRenderer, const std::string& iRendererApi,
                   ShaderType iStage, const std::vector<uint32_t>& iShaderData);
} // namespace owl::renderer::utils
