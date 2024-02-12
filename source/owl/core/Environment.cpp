/**
 * @file Environment.cpp
 * @author Silmaen
 * @date 13/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Environment.h"

namespace owl::core {

std::string getEnv(const std::string &key) {
#ifdef OWL_PLATFORM_WINDOWS
	size_t requiredSize;
	getenv_s(&requiredSize, nullptr, 0, key.c_str());
	if (requiredSize == 0)
		return "";
	std::string buffer;
	buffer.resize(requiredSize);
	getenv_s(&requiredSize, buffer.data(), requiredSize, key.c_str());
	return buffer;
#else
	const char *oldPath = getenv(key.c_str());
	if (oldPath == nullptr)
		return "";
	return oldPath;
#endif
}

void setEnv(const std::string &key, const std::string &value) {
#ifdef OWL_PLATFORM_WINDOWS
	_putenv_s(key.c_str(), value.c_str());
#else
	setenv(key.c_str(), value.c_str(), 1 /* overwrite */);
#endif
}

void appendEnv(const std::string &key, const std::string &value, const std::string &sepparator) {
	auto str = getEnv(key);
	if (str.empty())
		str = value;
	else
		str = value + sepparator + str;
	setEnv(key, str);
}

template<typename T>
void setEnvValue(const std::string &key, const T &value) {
	setEnv(key, fmt::format("{}", value));
}

template<typename T>
void appendEnvValue(const std::string &key, const T &value) {
	appendEnv(key, fmt::format("{}", value));
}

}// namespace owl::core
