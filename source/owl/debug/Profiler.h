/**
* @file Profiler.h
* @author Silmaen
* @date 17/08/2022
* Copyright © 2022 All rights reserved.
* All modification must get authorization from the author.
*/
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

/// The type for microseconds.
using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

/**
 * @brief Data for profiling result.
 */
struct ProfileResult {
	std::string name;                     /// Result's name.
	FloatingPointMicroseconds start;      /// Data's starting time point.
	std::chrono::microseconds elapsedTime;/// Data's elapsed time.
	std::thread::id threadID;             /// Data's thread ID.
};

/**
 * @brief Profile Session Data.
 */
struct ProfileSession {
	std::string name;/// Session's name.
};

/**
 * @brief class Profiler.
 */
class OWL_API Profiler {
public:
	Profiler(const Profiler &) = delete;
	Profiler(Profiler &&) = delete;
	Profiler &operator=(const Profiler &) = delete;
	Profiler &operator=(Profiler &&) = delete;

	/**
	 * @brief Begins a new profiling session.
	 * @param name_ Session's name.
	 * @param filepath Session File path to store information.
	 */
	void beginSession(const std::string &name_,
					  const std::string &filepath = "results.json") {
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

	/**
	 * @brief Terminate profile session.
	 */
	void endSession() {
		std::lock_guard<std::mutex> lock(profilerMutex);
		internalEndSession();
	}

	/**
	 * @brief Write profiling result into json file.
	 * @param result The Result to write.
	 */
	void writeProfile(const ProfileResult &result) {
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

	/**
	 * @brief Singleton accessor.
	 * @return This instance.
	 */
	static Profiler &get();

private:
	/**
	 * @brief Private Constructor.
	 */
	Profiler() : currentSession(nullptr) {}

	/**
	 * @brief Private destructor.
	 */
	~Profiler() { endSession(); }

	/**
	 * @brief write json header.
	 */
	void writeHeader() {
		outputStream << R"({"otherData": {},"traceEvents":[{})";
		outputStream.flush();
	}

	/**
	 * @brief write json footer.
	 */
	void writeFooter() {
		outputStream << "]}";
		outputStream.flush();
	}

	/**
	 * @brief Terminate the session.
	 *
	 * @note: you must already own lock on m_Mutex before calling InternalEndSession().
	 */
	void internalEndSession() {
		if (currentSession) {
			writeFooter();
			outputStream.close();
			delete currentSession;
			currentSession = nullptr;
		}
	}

	std::mutex profilerMutex;      /// Mutex.
	ProfileSession *currentSession;/// actual running session.
	std::ofstream outputStream;    /// output file stream.
};

/**
 * @brief Timer used for profiling.
 */
class ProfileTimer {
public:
	/**
	 * @brief Constructor.
	 * @param name_ Scope's name.
	 */
	explicit ProfileTimer(const char *name_) : name(name_), startTimePoint{std::chrono::steady_clock::now()},
											   stopped(false) {
	}

	/**
	 * @brief Destructor.
	 */
	~ProfileTimer() {
		if (!stopped)
			stop();
	}

	/**
	 * @brief Stop the timer.
	 */
	void stop() {
		auto endTimePoint = std::chrono::steady_clock::now();
		auto highResStart =
				FloatingPointMicroseconds{startTimePoint.time_since_epoch()};
		auto elapsedTime =
				std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint)
						.time_since_epoch() -
				std::chrono::time_point_cast<std::chrono::microseconds>(
						startTimePoint)
						.time_since_epoch();

		Profiler::get().writeProfile(
				{name, highResStart, elapsedTime, std::this_thread::get_id()});

		stopped = true;
	}

private:
	/// Scope's name.
	const char *name;
	/// Timer starting point.
	std::chrono::time_point<std::chrono::steady_clock> startTimePoint;
	/// Timer state, true if not running.
	bool stopped;
};

/**
 * @brief Namespace for profiling utility functions.
 */
namespace utils {

/**
 * @brief Simple char array.
 * @tparam N Size of the array.
 */
template<size_t N>
struct ChangeResult {
	char Data[N];
};

/**
 * @brief Simple String cleaner.
 * @tparam N Size of the string.
 * @tparam K Size of pattern to remove.
 * @param expr The string.
 * @param remove Pattern to remove.
 * @return The corrected string.
 */
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
}// namespace utils

}// namespace owl::debug

#ifndef OWL_PROFILE
#define OWL_PROFILE 0
#else
#undef OWL_PROFILE
#define OWL_PROFILE 1
#endif
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
	::owl::debug::Profiler::get().beginSession(name, filepath);
#define OWL_PROFILE_END_SESSION() ::owl::debug::Profiler::get().endSession();
#define OWL_PROFILE_SCOPE_LINE2(name, line)                             \
	constexpr auto fixedName##line =                                    \
			::owl::debug::utils::cleanupOutputString(name, "__cdecl "); \
	::owl::debug::ProfileTimer timer##line(fixedName##line.Data);
#define OWL_PROFILE_SCOPE_LINE(name, line) OWL_PROFILE_SCOPE_LINE2(name, line)
#define OWL_PROFILE_SCOPE(name) OWL_PROFILE_SCOPE_LINE(name, __LINE__)
#define OWL_PROFILE_FUNCTION() OWL_PROFILE_SCOPE(OWL_FUNC_SIG)
#else
#define OWL_PROFILE_BEGIN_SESSION(name, filepath)
#define OWL_PROFILE_END_SESSION()
#define OWL_PROFILE_SCOPE(name)
#define OWL_PROFILE_FUNCTION()
#endif
