#pragma once

#include "core/Log.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace owl::debug {

using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct ProfileResult {
	std::string name;
	FloatingPointMicroseconds start;
	std::chrono::microseconds elapsedTime;
	std::thread::id threadID;
};

struct InstrumentationSession {
	std::string name;
};

class Instrumentor {
public:
	Instrumentor(const Instrumentor &) = delete;
	Instrumentor(Instrumentor &&) = delete;
	Instrumentor &operator=(const Instrumentor &) = delete;
	Instrumentor &operator=(Instrumentor &&) = delete;

	void beginSession(const std::string &name_,
						const std::string &filepath = "results.json") {
		std::lock_guard lock(instrumentorMutex);
		if (currentSession) {
			// If there is already a current session, then close it before beginning
			// new one. Subsequent profiling output meant for the original session
			// will end up in the newly opened session instead.	That's better than
			// having badly formatted profiling output.

			// Edge case: BeginSession() might be
			// before Log::Init()
			if (core::Log::getCoreLogger()) {
				OWL_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' "
								 "already open.",
								 name_, currentSession->name);
			}
			internalEndSession();
		}
		outputStream.open(filepath);

		if (outputStream.is_open()) {
			currentSession = new InstrumentationSession({name_});
			writeHeader();
		} else {
			if (core::Log::getCoreLogger())// Edge case: BeginSession() might be
											 // before Log::Init()
			{
				OWL_CORE_ERROR("Instrumentor could not open results file '{0}'.",
								 filepath);
			}
		}
	}

	void endSession() {
		std::lock_guard lock(instrumentorMutex);
		internalEndSession();
	}

	void writeProfile(const ProfileResult &result) {
		std::stringstream json;

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << (result.elapsedTime.count()) << ',';
		json << "\"name\":\"" << result.name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.threadID << ",";
		json << "\"ts\":" << result.start.count();
		json << "}";

		std::lock_guard lock(instrumentorMutex);
		if (currentSession) {
			outputStream << json.str();
			outputStream.flush();
		}
	}

	static Instrumentor &get();

private:
	Instrumentor() : currentSession(nullptr) {}

	~Instrumentor() { endSession(); }

	void writeHeader() {
		outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
		outputStream.flush();
	}

	void writeFooter() {
		outputStream << "]}";
		outputStream.flush();
	}

	// Note: you must already own lock on m_Mutex before
	// calling InternalEndSession()
	void internalEndSession() {
		if (currentSession) {
			writeFooter();
			outputStream.close();
			delete currentSession;
			currentSession = nullptr;
		}
	}

	std::mutex instrumentorMutex;
	InstrumentationSession *currentSession;
	std::ofstream outputStream;
};

class InstrumentationTimer {
public:
	InstrumentationTimer(const char *name_) : name(name_), stopped(false) {
		startTimepoint = std::chrono::steady_clock::now();
	}

	~InstrumentationTimer() {
		if (!stopped)
			stop();
	}

	void stop() {
		auto endTimepoint = std::chrono::steady_clock::now();
		auto highResStart =
				FloatingPointMicroseconds{startTimepoint.time_since_epoch()};
		auto elapsedTime =
				std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint)
						.time_since_epoch() -
				std::chrono::time_point_cast<std::chrono::microseconds>(
						startTimepoint)
						.time_since_epoch();

		Instrumentor::get().writeProfile(
				{name, highResStart, elapsedTime, std::this_thread::get_id()});

		stopped = true;
	}

private:
	const char *name;
	std::chrono::time_point<std::chrono::steady_clock> startTimepoint;
	bool stopped;
};

namespace InstrumentorUtils {

template<size_t N>
struct ChangeResult {
	char Data[N];
};

template<size_t N, size_t K>
constexpr auto cleanupOutputString(const char (&expr)[N],
								   const char (&remove)[K]) {
	ChangeResult<N> result = {};

	size_t srcIndex = 0;
	size_t dstIndex = 0;
	while (srcIndex < N) {
		size_t matchIndex = 0;
		while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 &&
			   expr[srcIndex + matchIndex] == remove[matchIndex])
			matchIndex++;
		if (matchIndex == K - 1)
			srcIndex += matchIndex;
		result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
		srcIndex++;
	}
	return result;
}
}// namespace InstrumentorUtils

}// namespace owl::debug

#define OWL_PROFILE 0
#if OWL_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || \
		(defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define OWL_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define OWL_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define OWL_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || \
		(defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define OWL_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define OWL_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define OWL_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define OWL_FUNC_SIG __func__
#else
#define OWL_FUNC_SIG "OWL_FUNC_SIG unknown!"
#endif

#define OWL_PROFILE_BEGIN_SESSION(name, filepath) \
	::owl::debug::Instrumentor::get().beginSession(name, filepath)
#define OWL_PROFILE_END_SESSION() ::owl::debug::Instrumentor::get().endSession()
#define OWL_PROFILE_SCOPE_LINE2(name, line)                                         \
	constexpr auto fixedName##line =                                                \
			::owl::debug::InstrumentorUtils::cleanupOutputString(name, "__cdecl "); \
	::owl::debug::InstrumentationTimer timer##line(fixedName##line.Data)
#define OWL_PROFILE_SCOPE_LINE(name, line) OWL_PROFILE_SCOPE_LINE2(name, line)
#define OWL_PROFILE_SCOPE(name) OWL_PROFILE_SCOPE_LINE(name, __LINE__)
#define OWL_PROFILE_FUNCTION() OWL_PROFILE_SCOPE(OWL_FUNC_SIG)
#else
#define OWL_PROFILE_BEGIN_SESSION(name, filepath)
#define OWL_PROFILE_END_SESSION()
#define OWL_PROFILE_SCOPE(name)
#define OWL_PROFILE_FUNCTION()
#endif