/**
 * @file ShaderLibrary.cpp
 * @author Silmaen
 * @date 17/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "ShaderLibrary.h"

namespace owl::renderer {


void ShaderLibrary::add(const shrd<Shader> &shader) {
	if (exists(shader->getName())){
		OWL_CORE_WARN("Shader {} Already in the library", shader->getName())
		return ;
	}
	shaders[shader->getName()] = shader;
}

void ShaderLibrary::addNRename(const std::string &name, const shrd<Shader> &shader) {
	if (exists(name)){
		OWL_CORE_WARN("Shader {} Already in the library", name)
		return ;
	}
	shader->name =name;
	shaders[name] = shader;
}

void ShaderLibrary::addFromStandardPath(const std::string &name) {
	if (exists(name)){
		OWL_CORE_WARN("Shader {} Already in the library", name)
		return ;
	}
	shaders[name] = Shader::create(name);
}

shrd<Shader> ShaderLibrary::load(const std::filesystem::path &file) {
	shrd<Shader> shader = Shader::create(file);
	if(shader == nullptr){
		OWL_CORE_WARN("Could not load shader file {}", file.string())
		return nullptr;
	}
	shaders[shader->getName()] = shader;
	return shader;
}
shrd<Shader> ShaderLibrary::get(const std::string &name) {
	OWL_CORE_ASSERT(exists(name),"Shader {} not found in library",name)
	return shaders[name];
}
bool ShaderLibrary::exists(const std::string &name) const {
	return shaders.find(name) != shaders.end();

}


}// namespace renderer
