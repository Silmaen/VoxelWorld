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
OWL_DIAG_PUSH
OWL_DIAG_DISABLE_CLANG("-Wformat-nonliteral")
#include "glm/gtx/string_cast.hpp"
OWL_DIAG_POP
/**
 * @brief Namespace for the core objects.
 */
namespace owl::core {

/// Default frequency for frame ooutput.
constexpr uint64_t gDefaultFrequency{100};

/**
 * @brief Logging system.
 */
class OWL_API Log {
public:
	/**
	 * @brief initialize the logging system.
	 * @param[in] iLevel Verbosity level of the logger.
	 * @param[in] iFrequency Frequence of frame outputput (number of frames).
	 */
	static void init(const spdlog::level::level_enum &iLevel = spdlog::level::trace,
					 uint64_t iFrequency = gDefaultFrequency);

	/**
	 * @brief Access to the logger for the core system.
	 * @return The Core logger.
	 */
	static shared<spdlog::logger> getCoreLogger() { return s_coreLogger; }

	/**
	 * @brief Access to the logger for the application system.
	 * @return The application logger.
	 */
	static shared<spdlog::logger> getClientLogger() { return s_clientLogger; }

	/**
	 * @brief Defines the Verbosity level
	 * @param[in] iLevel Verbosity level.
	 */
	static void setVerbosityLevel(const spdlog::level::level_enum &iLevel);

	/**
	 * @brief Destroy the logger.
	 */
	static void invalidate();

	/**
	 * @brief Check if logger is initiated.
	 * @return True if initiated.
	 */
	static bool initiated() { return s_coreLogger != nullptr; }

	/**
	 * @brief To know if in logging frame.
	 * @return True if in logging frame.
	 */
	static bool frameLog() { return s_frequency > 0 && s_frameCounter % s_frequency == 0; }

	/**
	 * @brief Start a new logging frame.
	 */
	static void newFrame();

	/**
	 * @brief define a new frame log frequncy.
	 * @param[in] iFrequency New frequency.
	 */
	static void setFrameFrequency(const uint64_t iFrequency) { s_frequency = iFrequency; }

private:
	/// The core logger.
	static shared<spdlog::logger> s_coreLogger;
	/// The application logger.
	static shared<spdlog::logger> s_clientLogger;
	/// The level of verbosity.
	static spdlog::level::level_enum s_verbosity;
	/// Counter for the frames.
	static uint64_t s_frameCounter;
	/// Frequency of frame trace.
	static uint64_t s_frequency;
};
}// namespace owl::core

/**
 * @brief Overload stream operator for vectors.
 * @tparam OStream The stream type.
 * @tparam L The vector's length type.
 * @tparam T The vector's component type.
 * @tparam Q The vector's qualifier type.
 * @param[out] oStream The stream to write onto.
 * @param[in] iVector The vector to write.
 * @return The actualized stream.
 */
template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
OStream &operator<<(OStream &oStream, const glm::vec<L, T, Q> &iVector) {
	return oStream << glm::to_string(iVector);
}

/**
 * @brief Overload stream operator for matrices.
 * @tparam OStream The stream type.
 * @tparam C The matrix's column length type.
 * @tparam R The matrix's Row length type.
 * @tparam T The matrix's component type.
 * @tparam Q The matrix's qualifier type.
 * @param[out] oStream The stream to write onto.
 * @param[in] iMatrix The matrix to write.
 * @return The actualized stream.
 */
template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
OStream &operator<<(OStream &oStream, const glm::mat<C, R, T, Q> &iMatrix) {
	return oStream << glm::to_string(iMatrix);
}

/**
 * @brief Overload stream operator for quaternions.
 * @tparam OStream The stream type.
 * @tparam T The quaternion's component type.
 * @tparam Q The quaternion's qualifier type.
 * @param[out] oStream The stream to write onto.
 * @param[in] iQuaternion The quaternion to write.
 * @return The actualized stream.
 */
template<typename OStream, typename T, glm::qualifier Q>
OStream &operator<<(OStream &oStream, glm::qua<T, Q> iQuaternion) {
	return oStream << glm::to_string(iQuaternion);
}

// Core log macros;
#define OWL_CORE_FRAME_TRACE(...)                                                                                      \
	if (::owl::core::Log::frameLog()) {                                                                                \
		::owl::core::Log::getCoreLogger()->trace(__VA_ARGS__);                                                         \
	}
#define OWL_CORE_FRAME_ADVANCE ::owl::core::Log::newFrame();

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
