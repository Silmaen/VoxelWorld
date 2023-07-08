/**
 * @file Log.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Core.h"
#include "external/spdlog.h"

#define GLM_ENABLE_EXPERIMENTAL
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif
#include "glm/gtx/string_cast.hpp"
#ifdef __clang__
#pragma clang diagnostic pop
#endif
/**
 * @brief Namespace for the core objects.
 */
namespace owl::core {

/**
 * @brief Class for the logging system.
 */
class OWL_API Log {
public:
	/**
	 * @brief initialize the logging system.
	 */
	static void init();
	/**
	 * @brief Access to the logger for the core system.
	 * @return The Core logger.
	 */
	static std::shared_ptr<spdlog::logger> getCoreLogger() { return coreLogger; }
	/**
	 * @brief Access to the logger for the application system.
	 * @return The application logger.
	 */
	static std::shared_ptr<spdlog::logger> getClientLogger() { return clientLogger; }

private:
	/// The core logger.
	static std::shared_ptr<spdlog::logger> coreLogger;
	/// The application logger.
	static std::shared_ptr<spdlog::logger> clientLogger;
};
}// namespace owl::core

/**
 * @brief Overload stream operator for vectors.
 * @tparam OStream The stream type.
 * @tparam L The vector's length type.
 * @tparam T The vector's component type.
 * @tparam Q The vector's qualifier type.
 * @param os The stream to write onto.
 * @param vector The vector to write.
 * @return The actualized stream.
 */
template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream &operator<<(OStream &os, const glm::vec<L, T, Q> &vector) {
	return os << glm::to_string(vector);
}

/**
 * @brief Overload stream operator for matrices.
 * @tparam OStream The stream type.
 * @tparam C The matrix's column length type.
 * @tparam R The matrix's Row length type.
 * @tparam T The matrix's component type.
 * @tparam Q The matrix's qualifier type.
 * @param os The stream to write onto.
 * @param matrix The matrix to write.
 * @return The actualized stream.
 */
template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream &operator<<(OStream &os, const glm::mat<C, R, T, Q> &matrix) {
	return os << glm::to_string(matrix);
}

/**
 * @brief Overload stream operator for quaternions.
 * @tparam OStream The stream type.
 * @tparam T The quaternion's component type.
 * @tparam Q The quaternion's qualifier type.
 * @param os The stream to write onto.
 * @param quaternion The quaternion to write.
 * @return The actualized stream.
 */
template<typename OStream, typename T, glm::qualifier Q>
inline OStream &operator<<(OStream &os, glm::qua<T, Q> quaternion) {
	return os << glm::to_string(quaternion);
}

// Core log macros
#define OWL_CORE_TRACE(...) ::owl::core::Log::getCoreLogger()->trace(__VA_ARGS__);
#define OWL_CORE_INFO(...) ::owl::core::Log::getCoreLogger()->info(__VA_ARGS__);
#define OWL_CORE_WARN(...) ::owl::core::Log::getCoreLogger()->warn(__VA_ARGS__);
#define OWL_CORE_ERROR(...) ::owl::core::Log::getCoreLogger()->error(__VA_ARGS__);
#define OWL_CORE_CRITICAL(...) ::owl::core::Log::getCoreLogger()->critical(__VA_ARGS__);

// Client log macros
#define OWL_TRACE(...) ::owl::core::Log::getClientLogger()->trace(__VA_ARGS__);
#define OWL_INFO(...) ::owl::core::Log::getClientLogger()->info(__VA_ARGS__);
#define OWL_WARN(...) ::owl::core::Log::getClientLogger()->warn(__VA_ARGS__);
#define OWL_ERROR(...) ::owl::core::Log::getClientLogger()->error(__VA_ARGS__);
#define OWL_CRITICAL(...) ::owl::core::Log::getClientLogger()->critical(__VA_ARGS__);
