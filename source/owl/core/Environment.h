/**
 * @file Environment.h
 * @author Silmaen
 * @date 13/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

namespace owl::core {

#ifdef OWL_PLATFORM_WINDOWS
constexpr char sep[] = ";";
#else
constexpr char sep[] = ":";
#endif

/**
 * @brief Get an anvironment variable value.
 * @param key The Environment vaiable's name.
 * @return Value or empty string.
 */
std::string getEnv(const std::string &key);

/**
 * @brief Define (overwrite) an environment variable.
 * @param key The Environment vaiable's name.
 * @param value The new value.
 */
void setEnv(const std::string &key, const std::string &value);

/**
 * @brief Append a value to existing environment variable.
 * @param key The Environment vaiable's name.
 * @param value The value to add.
 * @param sepparator The sepparator between values.
 */
void appendEnv(const std::string &key, const std::string &value, const std::string &sepparator = sep);

/**
 * @brief Define (overwrite) an environment variable.
 * @tparam T The type of value.
 * @param key The Environment vaiable's name.
 * @param value The new value.
 */
template<typename T>
void setEnvValue(const std::string &key, const T &value);

/**
 * @brief Append a value to existing environment variable.
 * @tparam T The type of value.
 * @param key The Environment vaiable's name.
 * @param value The value to add.
 */
template<typename T>
void appendEnvValue(const std::string &key, const T &value);

}// namespace owl::core
