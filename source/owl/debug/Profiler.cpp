/**
 * @file Profiler.cpp
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Profiler.h"

namespace owl::debug {

Profiler::Profiler() : currentSession(nullptr) {}

Profiler::~Profiler() { endSession(); }

void Profiler::beginSession(const std::string &name_,
							const std::string &filepath) {
	std::lock_guard<std::mutex> lock(profilerMutex);
	if (currentSession) {
		// If there is already a current session, then close it before beginning
		// new one. Subsequent profiling output meant for the original session
		// will end up in the newly opened session instead.	That's better than
		// having badly formatted profiling output.

		// Edge case: BeginSession() might be
		// before Log::Init()
		if (core::Log::getCoreLogger()) {
			OWL_CORE_ERROR("Profiler::BeginSession('{0}') when session '{1}' "
						   "already open.",
						   name_, currentSession->name)
		}
		internalEndSession();
	}
	outputStream.open(filepath);

	if (outputStream.is_open()) {
		currentSession = new ProfileSession({name_});
		writeHeader();
	} else {
		if (core::Log::getCoreLogger())// Edge case: BeginSession() might be
									   // before Log::Init()
		{
			OWL_CORE_ERROR("Instrumentor could not open results file '{0}'.",
						   filepath)
		}
	}
}

void Profiler::endSession() {
	std::lock_guard<std::mutex> lock(profilerMutex);
	internalEndSession();
}

void Profiler::writeProfile(const ProfileResult &result) {
	std::stringstream json;

	json << std::setprecision(3) << std::fixed;
	json << ",{";
	json << R"("cat":"function",)";
	json << "\"dur\":" << (result.elapsedTime.count()) << ',';
	json << R"("name":")" << result.name << "\",";
	json << R"("ph":"X",)";
	json << "\"pid\":0,";
	json << "\"tid\":" << result.threadID << ",";
	json << "\"ts\":" << result.start.count();
	json << "}";

	std::lock_guard<std::mutex> lock(profilerMutex);
	if (currentSession) {
		outputStream << json.str();
		outputStream.flush();
	}
}

void Profiler::writeHeader() {
	outputStream << R"({"otherData": {},"traceEvents":[{})";
	outputStream.flush();
}

void Profiler::writeFooter() {
	outputStream << "]}";
	outputStream.flush();
}

void Profiler::internalEndSession() {
	if (currentSession) {
		writeFooter();
		outputStream.close();
		delete currentSession;
		currentSession = nullptr;
	}
}

ProfileTimer::ProfileTimer(const char *name_) : name(name_), startTimePoint{std::chrono::steady_clock::now()},
												stopped(false) {
}

ProfileTimer::~ProfileTimer() {
	if (!stopped)
		stop();
}

void ProfileTimer::stop() {
	const auto endTimePoint = std::chrono::steady_clock::now();
	const auto highResStart =
			FloatingPointMicroseconds{startTimePoint.time_since_epoch()};
	const auto elapsedTime =
			std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint)
					.time_since_epoch() -
			std::chrono::time_point_cast<std::chrono::microseconds>(
					startTimePoint)
					.time_since_epoch();

	Profiler::get().writeProfile(
			{name, highResStart, elapsedTime, std::this_thread::get_id()});

	stopped = true;
}

}// namespace owl::debug
