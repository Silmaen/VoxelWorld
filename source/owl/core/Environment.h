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
constexpr char g_sep[] = ";";
#else
constexpr char g_sep[] = ":";
#endif

/**
 * @brief Get an anvironment variable value.
 * @param[in] iKey The Environment vaiable's name.
 * @return Value or empty string.
 */
std::string getEnv(const std::string &iKey);

/**
 * @brief Define (overwrite) an environment variable.
 * @param[in] iKey The Environment vaiable's name.
 * @param[in] iValue The new value.
 */
void setEnv(const std::string &iKey, const std::string &iValue);

/**
 * @brief Append a value to existing environment variable.
 * @param[in] iKey The Environment vaiable's name.
 * @param[in] iValue The value to add.
 * @param[in] iSepparator The sepparator between values.
 */
void appendEnv(const std::string &iKey, const std::string &iValue, const std::string &iSepparator = g_sep);

/**
 * @brief Define (overwrite) an environment variable.
 * @tparam T The type of value.
 * @param[in] iKey The Environment vaiable's name.
 * @param[in] iValue The new value.
 */
template<typename T>
void setEnvValue(const std::string &iKey, const T &iValue);

/**
 * @brief Append a value to existing environment variable.
 * @tparam T The type of value.
 * @param[in] iKey The Environment vaiable's name.
 * @param[in] iValue The value to add.
 */
template<typename T>
void appendEnvValue(const std::string &iKey, const T &iValue);

}// namespace owl::core
