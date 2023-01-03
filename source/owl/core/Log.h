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

/**
 * @brief Namespace for the core objects
 */
namespace owl::core {
/**
 * @brief Class Log
 */
class OWL_API Log {
public:
	static void init();
	static std::shared_ptr<spdlog::logger> getCoreLogger() { return coreLogger; }
	static std::shared_ptr<spdlog::logger> getClientLogger() { return clientLogger; }

private:
	static std::shared_ptr<spdlog::logger> coreLogger;
	static std::shared_ptr<spdlog::logger> clientLogger;
};
}// namespace owl::core

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
