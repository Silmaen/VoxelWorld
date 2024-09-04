/**
 * @file Log.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Macros.h"
#include "Memory.h"

#include "external/spdlog.h"

/**
 * @brief Namespace for the core objects.
 */
namespace owl::core {

/// Default frequency for frame output.
constexpr uint64_t g_DefaultFrequency{100};

/**
 * @brief Logging system.
 */
class OWL_API Log {
public:
	/**
	 * @brief initialize the logging system.
	 * @param[in] iLevel Verbosity level of the logger.
	 * @param[in] iFrequency Frequency of frame output (number of frames).
	 */
	static void init(const spdlog::level::level_enum& iLevel = spdlog::level::trace,
					 uint64_t iFrequency = g_DefaultFrequency);

	/**
	 * @brief Access to the logger for the core system.
	 * @return The Core logger.
	 */
	static auto getCoreLogger() -> shared<spdlog::logger> { return s_coreLogger; }

	/**
	 * @brief Access to the logger for the application system.
	 * @return The application logger.
	 */
	static auto getClientLogger() -> shared<spdlog::logger> { return s_clientLogger; }

	/**
	 * @brief Defines the Verbosity level
	 * @param[in] iLevel Verbosity level.
	 */
	static void setVerbosityLevel(const spdlog::level::level_enum& iLevel);

	/**
	 * @brief Destroy the logger.
	 */
	static void invalidate();

	/**
	 * @brief Check if logger is initiated.
	 * @return True if initiated.
	 */
	static auto initiated() -> bool { return s_coreLogger != nullptr; }

	/**
	 * @brief To know if in logging frame.
	 * @return True if in logging frame.
	 */
	static auto frameLog() -> bool { return s_frequency > 0 && s_frameCounter % s_frequency == 0; }

	/**
	 * @brief Start a new logging frame.
	 */
	static void newFrame();

	/**
	 * @brief define a new frame log frequency.
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
