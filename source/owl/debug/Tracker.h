/**
 * @file Tracker.h
 * @author Silmaen
 * @date 17/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Macros.h"
#include <list>

#ifndef OWL_TRACKER_VERBOSITY
#define OWL_TRACKER_VERBOSITY 0
#endif

#if defined(OWL_DEBUG) && !defined(OWL_COVERAGE)
#if OWL_TRACKER_VERBOSITY < 1
#undef OWL_TRACKER_VERBOSITY
#define OWL_TRACKER_VERBOSITY 1
#endif
#endif

#ifdef OWL_STACKTRACE
#if OWL_TRACKER_VERBOSITY < 3
#undef OWL_TRACKER_VERBOSITY
#define OWL_TRACKER_VERBOSITY 3
#endif
#include <cpptrace/cpptrace.hpp>
#endif

using size_t = std::size_t;

/**
 * @namespace owl
 * @brief Base namespace for the project.
 */

/**
 * @brief namespace for debug functions.
 */
namespace owl::debug {
/**
 * @brief Information about memory chunk.
 */
struct OWL_API AllocationInfo {
	AllocationInfo(void* iLocation, size_t iSize);
	/// Location in memory.
	void* location = nullptr;
	/// Size of the memory chunk.
	size_t size = 0;
#ifdef OWL_STACKTRACE
	/// Stack trace of the allocation.
	cpptrace::stacktrace fullTrace;
	/// @brief Look in the stack for the caller information.
	/// @return The Calling frame.
	[[nodiscard]] auto getCallerInfo() const -> const cpptrace::stacktrace_frame&;
#endif
	/**
	 * @brief Express this allocation line as a string
	 * @param[in] iTracePrint If the simplified trace should be print.
	 * @param[in] iFullTrace If we print the full stack trace instead of simplified one.
	 * @return String of the allocation.
	 */
	[[nodiscard]] OWL_API auto toStr(bool iTracePrint = true, bool iFullTrace = false) const -> std::string;
};

/**
 * @brief Result structure of allocation state.
 */
struct OWL_API AllocationState {
	~AllocationState();
	AllocationState() = default;
	AllocationState(const AllocationState&) = default;
	AllocationState(AllocationState&&) = default;
	auto operator=(const AllocationState&) -> AllocationState& = default;
	auto operator=(AllocationState&&) -> AllocationState& = default;
	/// Amount of allocated memory.
	size_t allocatedMemory = 0;
	/// Amount of memory allocation calls.
	size_t allocationCalls = 0;
	/// Amount of de-allocation calls.
	size_t deallocationCalls = 0;
	/// Max seen amount of memory.
	size_t memoryPeek = 0;
	/// list of allocated chunks of memory.
	std::list<AllocationInfo> allocs;
	/**
	 * @brief Reset the database.
	 */
	void reset();
	/**
	 * @brief Add a chunk of memory to the database.
	 * @param[in] iLocation Pointer to the allocated memory
	 * @param[in] iSize Amount of allocated memory.
	 */
	void pushMemory(void* iLocation, size_t iSize);
	/**
	 * @brief Free the chunk of memory at the given location.
	 * @param[in] iLocation Memory location.
	 * @param[in] iSize Amount of memory.
	 */
	void freeMemory(void* iLocation, size_t iSize);
};

class OWL_API TrackerAPI {
public:
	/**
	 * @brief Function called at each allocation.
	 * @param[in] iMemoryPtr Memory pointer where allocation is done.
	 * @param[in] iSize The Allocated size.
	 */
	static void allocate(void* iMemoryPtr, size_t iSize);

	/**
	 * @brief Function called each de-allocation.
	 * @param[in] iMemoryPtr Memory pointer to deallocate.
	 * @param[in] iSize De-allocation size.
	 */
	static void deallocate(void* iMemoryPtr, size_t iSize = 0);

	/**
	 * @brief Reset current memory state monitor and give the previous status.
	 * @return Status since last call to check.
	 */
	static auto checkState() -> const AllocationState&;

	/**
	 * @brief Get the memory state since the start of the program.
	 * @return Memory state.
	 */
	static auto globals() -> const AllocationState&;
};

// ---------- Scope utilities --------------------

/**
 * @brief Simple class that disable memory tracking during its lifetime.
 */
class OWL_API ScopeUntrack {
public:
	/// Constructor.
	ScopeUntrack();
	/// Destructor.
	~ScopeUntrack();

	ScopeUntrack(const ScopeUntrack&) = delete;
	ScopeUntrack(ScopeUntrack&&) = delete;
	auto operator=(const ScopeUntrack&) -> ScopeUntrack& = delete;
	auto operator=(ScopeUntrack&&) -> ScopeUntrack& = delete;
};
/**
* @brief Simple class that disable memory tracking during its lifetime.
*/
class OWL_API ScopeTrack {
public:
	/// Constructor.
	ScopeTrack();
	/// Destructor.
	~ScopeTrack();

	ScopeTrack(const ScopeTrack&) = delete;
	ScopeTrack(ScopeTrack&&) = delete;
	auto operator=(const ScopeTrack&) -> ScopeTrack& = delete;
	auto operator=(ScopeTrack&&) -> ScopeTrack& = delete;
};

}// namespace owl::debug

#define OWL_SCOPE_UNTRACK const owl::debug::ScopeUntrack scopeUntrack;
#define OWL_SCOPE_FORCE_TRACK const owl::debug::ScopeTrack scopeTrack;
