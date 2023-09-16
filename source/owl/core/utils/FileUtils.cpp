/**
 * @file FileUtils.cpp
 * @author Silmaen
 * @date 30/08/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "FileUtils.h"

namespace owl::core::utils {

std::string fileToString(const std::filesystem::path &file) {
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

}// namespace owl::core::utils