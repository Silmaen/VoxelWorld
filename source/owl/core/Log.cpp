/**
 * @file Log.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Log.h"
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#pragma clang diagnostic ignored "-Wundefined-func-template"
#endif
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace owl::core {

std::shared_ptr<spdlog::logger> Log::coreLogger;
std::shared_ptr<spdlog::logger> Log::clientLogger;

void Log::init() {
	std::vector<spdlog::sink_ptr> logSinks;
	logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Owl.log", true));

	logSinks[0]->set_pattern("%^[%T] %n: %v%$");
	logSinks[1]->set_pattern("[%T] [%l] %n: %v");

	coreLogger = std::make_shared<spdlog::logger>("OWL", begin(logSinks), end(logSinks));
	spdlog::register_logger(coreLogger);
	coreLogger->set_level(spdlog::level::trace);
	coreLogger->flush_on(spdlog::level::trace);

	clientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
	spdlog::register_logger(clientLogger);
	clientLogger->set_level(spdlog::level::trace);
	clientLogger->flush_on(spdlog::level::trace);
}

}// namespace owl::core
