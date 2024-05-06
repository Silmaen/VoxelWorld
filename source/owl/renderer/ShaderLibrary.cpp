/**
 * @file ShaderLibrary.cpp
 * @author Silmaen
 * @date 17/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "ShaderLibrary.h"

namespace owl::renderer {


void ShaderLibrary::add(const shared<Shader> &iShader) {
	if (exists(iShader->getName(), iShader->getRenderer())) {
		OWL_CORE_WARN("Shader {} Already in the library", iShader->getName())
		return;
	}
	m_shaders[iShader->getFullName()] = iShader;
}

void ShaderLibrary::addNRename(const std::string &iName, const std::string &iRenderer, const shared<Shader> &iShader) {
	if (exists(iName, iRenderer)) {
		OWL_CORE_WARN("Shader {} Already in the library", iName)
		return;
	}
	iShader->m_name = iName;
	iShader->m_renderer = iRenderer;
	m_shaders[iShader->getFullName()] = iShader;
}

void ShaderLibrary::addFromStandardPath(const std::string &iName, const std::string &iRenderer) {
	if (exists(iName, iRenderer)) {
		OWL_CORE_WARN("Shader {} Already in the library", iName)
		return;
	}
	if (iRenderer.empty())
		m_shaders[iName] = Shader::create(iName, iRenderer);
	else
		m_shaders[fmt::format("{}_{}", iRenderer, iName)] = Shader::create(iName, iRenderer);
}

shared<Shader> ShaderLibrary::load(const std::string &iName, const std::string &iRenderer,
								   const std::filesystem::path &iFile) {
	shared<Shader> shader = Shader::create(iName, iRenderer, iFile);
	if (shader == nullptr) {
		OWL_CORE_WARN("Could not load shader file {}", iFile.string())
		return nullptr;
	}
	m_shaders[shader->getName()] = shader;
	return shader;
}

shared<Shader> ShaderLibrary::get(const std::string &iName, const std::string &iRenderer) {
	if (!exists(iName, iRenderer)) {
		OWL_CORE_ERROR("Shader {} not found in library", iName)
		return nullptr;
	}
	return m_shaders[fmt::format("{}_{}", iRenderer, iName)];
}

bool ShaderLibrary::exists(const std::string &iName, const std::string &iRenderer) const {
	return m_shaders.contains(fmt::format("{}_{}", iRenderer, iName));
}

ShaderLibrary::~ShaderLibrary() {
	for (auto &shader: m_shaders) shader.second.reset();
	m_shaders.clear();
}

}// namespace owl::renderer
