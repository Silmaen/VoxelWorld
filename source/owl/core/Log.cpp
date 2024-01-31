/**
 * @file Log.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Log.h"
OWL_DIAG_PUSH
OWL_DIAG_DISABLE_CLANG("-Wweak-vtables")
OWL_DIAG_DISABLE_CLANG("-Wundefined-func-template")
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
OWL_DIAG_POP
#include "debug/Tracker.h"

namespace owl::core {

std::shared_ptr<spdlog::logger> Log::coreLogger;
std::shared_ptr<spdlog::logger> Log::clientLogger;
spdlog::level::level_enum Log::verbosity = spdlog::level::trace;

void Log::init(const spdlog::level::level_enum &level) {
	OWL_SCOPE_UNTRACK
	if (coreLogger != nullptr) {
		OWL_CORE_INFO("Logger already initiated.")
		return;
	}
	std::vector<spdlog::sink_ptr> logSinks;
	logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
#ifdef WIN32
	logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(L"Owl.log", true));
#else
	logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Owl.log", true));
#endif

	logSinks[0]->set_pattern("%^[%T] %n: %v%$");
	logSinks[1]->set_pattern("[%T] [%l] %n: %v");

	coreLogger = std::make_shared<spdlog::logger>("OWL", begin(logSinks), end(logSinks));
	spdlog::register_logger(coreLogger);

	clientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
	spdlog::register_logger(clientLogger);
	setVerbosityLevel(level);
}

void Log::setVerbosityLevel(const spdlog::level::level_enum &level) {
	verbosity = level;
	if (coreLogger) {
		coreLogger->set_level(verbosity);
		coreLogger->flush_on(verbosity);
	}
	if (clientLogger) {
		clientLogger->set_level(verbosity);
		clientLogger->flush_on(verbosity);
	}
}

void Log::invalidate() {
	OWL_SCOPE_UNTRACK
	spdlog::drop_all();
	coreLogger.reset();
	clientLogger.reset();
}

}// namespace owl::core
