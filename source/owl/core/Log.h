/**
 * @file Log.h
 * @author argawaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>

namespace owl::core {
/**
 * @brief Class Log
 */
class OWL_API Log {
public:
  static void init();
  static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger;}
  static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger;}
private:
  static std::shared_ptr<spdlog::logger> s_CoreLogger;
  static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
}

// Core log macros
#define OWL_CORE_TRACE(...)    ::owl::core::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OWL_CORE_INFO(...)     ::owl::core::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OWL_CORE_WARN(...)     ::owl::core::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OWL_CORE_ERROR(...)    ::owl::core::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OWL_CORE_CRITICAL(...) ::owl::core::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define OWL_TRACE(...)         ::owl::core::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OWL_INFO(...)          ::owl::core::Log::GetClientLogger()->info(__VA_ARGS__)
#define OWL_WARN(...)          ::owl::core::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OWL_ERROR(...)         ::owl::core::Log::GetClientLogger()->error(__VA_ARGS__)
#define OWL_CRITICAL(...)      ::owl::core::Log::GetClientLogger()->critical(__VA_ARGS__)
