/**
 * @file ShaderLibrary.cpp
 * @author Silmaen
 * @date 17/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "ShaderLibrary.h"

namespace owl::renderer {


void ShaderLibrary::add(const shared<Shader> &shader) {
	if (exists(shader->getName(), shader->getRenderer())) {
		OWL_CORE_WARN("Shader {} Already in the library", shader->getName())
		return;
	}
	shaders[shader->getFullName()] = shader;
}

void ShaderLibrary::addNRename(const std::string &name, const std::string &renderer, const shared<Shader> &shader) {
	if (exists(name, renderer)) {
		OWL_CORE_WARN("Shader {} Already in the library", name)
		return;
	}
	shader->name = name;
	shader->renderer = renderer;
	shaders[shader->getFullName()] = shader;
}

void ShaderLibrary::addFromStandardPath(const std::string &name, const std::string &renderer) {
	if (exists(name, renderer)) {
		OWL_CORE_WARN("Shader {} Already in the library", name)
		return;
	}
	if (renderer.empty())
		shaders[name] = Shader::create(name, renderer);
	else
		shaders[fmt::format("{}_{}", renderer, name)] = Shader::create(name, renderer);
}

shared<Shader> ShaderLibrary::load(const std::string &name, const std::string &renderer, const std::filesystem::path &file) {
	shared<Shader> shader = Shader::create(name, renderer, file);
	if (shader == nullptr) {
		OWL_CORE_WARN("Could not load shader file {}", file.string())
		return nullptr;
	}
	shaders[shader->getName()] = shader;
	return shader;
}

shared<Shader> ShaderLibrary::get(const std::string &name, const std::string &renderer) {
	if (!exists(name, renderer)) {
		OWL_CORE_ERROR("Shader {} not found in library", name)
		return nullptr;
	}
	return shaders[fmt::format("{}_{}", renderer, name)];
}

bool ShaderLibrary::exists(const std::string &name, const std::string &renderer) const {
	return shaders.find(fmt::format("{}_{}", renderer, name)) != shaders.end();
}

ShaderLibrary::~ShaderLibrary() {
	for (auto &shader: shaders)
		shader.second.reset();
	shaders.clear();
}

}// namespace owl::renderer
