/**
 * @file Tracker.h
 * @author Silmaen
 * @date 17/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

#if defined(OWL_DEBUG) && !defined(OWL_COVERAGE)
#define OWL_STACKTRACE
#endif

#ifdef OWL_STACKTRACE
#include <cpptrace/cpptrace.hpp>
#endif
#include <list>

using size_t = std::size_t;

/**
 * @namespace owl
 * @brief Base namespace for the project.
 */

/**
 * @brief namespace for debug functions.
 */
namespace owl::debug {

#ifdef OWL_STACKTRACE
/**
 * @brief Simple class that enable Stacktrace during its lifetime.
 */
class OWL_API ScopeTrace {
public:
	/// Constructor.
	ScopeTrace();
	/// Destructor.
	~ScopeTrace();
};
#endif

/**
 * @brief Simple class that disable memory tracking during its lifetime.
 */
class OWL_API ScopeUntrack {
public:
	/// Constructor.
	ScopeUntrack();
	/// Destructor.
	~ScopeUntrack();
};

/**
 * @brief Class Tracker.
 *
 * A simple memory allocation tracker.
 */
class OWL_API Tracker {
public:
	Tracker(const Tracker &) = delete;
	Tracker(Tracker &&) = delete;
	Tracker &operator=(const Tracker &) = delete;
	Tracker &operator=(Tracker &&) = delete;
	/**
	 * @brief Destructor.
	 */
	~Tracker();

	/**
	 * @brief Get engine instance.
	 * @return The engine instance.
	 */
	static Tracker &get();

	/**
	 * @brief Function called at each allocation.
	 * @param memoryPtr Memory pointer where allocation is done.
	 * @param size The Allocated size.
	 */
	void allocate(void *memoryPtr, size_t size);

	/**
	 * @brief Function called each de-allocation.
	 * @param memoryPtr Memory pointer to deallocate.
	 * @param size De-allocation size.
	 */
	void deallocate(void *memoryPtr, size_t size = 0);

	/**
	 * @brief Information about memory chunk.
	 */
	struct AllocationInfo {
		AllocationInfo(void *bob, size_t bobette) : location{bob}, size{bobette} {}
		void *location = nullptr;///< location in memory.
		size_t size = 0;         ///< size of the memory chunk.
#ifdef OWL_STACKTRACE
		cpptrace::stacktrace_frame traceAlloc;///< Stack trace of the allocation.
#endif
		/**
		 * @brief Express this allocation line as a string
		 * @return String of the allocation.
		 */
		[[nodiscard]] OWL_API std::string toStr() const;
	};

	/**
	 * @brief Result structure of allocation state.
	 */
	struct AllocationState {
		size_t allocatedMemory = 0;      ///< Amount of allocated memory.
		size_t allocationCalls = 0;      ///< Amount of memory allocation calls.
		size_t deallocationCalls = 0;    ///< Amount of de-allocation calls.
		size_t memoryPeek = 0;           ///< Max seen amount of memory.
		std::list<AllocationInfo> allocs;///< list of allocated chunks of memory.
		/**
		 * @brief Add a chunk of memory to the database.
		 * @param nemPtr Pointer to the allocated memory
		 * @param size Amount of allocated memory.
		 */
		void pushMemory(void *nemPtr, size_t size);
		/**
		 * @brief Free the chunk of memory at the given location.
		 * @param nemPtr Memory location.
		 * @param size Amount of memory.
		 */
		void freeMemory(void *nemPtr, size_t size);
		/**
		 * @brief Reset the database.
		 */
		void reset();
	};

	/**
	 * @brief Reset current memory state monitor and give the previous status.
	 * @return Status since last call to check.
	 */
	const AllocationState &checkState();

	/**
	 * @brief Get the memory state since the start of the program.
	 * @return Memory state.
	 */
	[[nodiscard]] const AllocationState &globals() const;

private:
	/**
	 * @brief Default constructor.
	 */
	Tracker();

	/// Global Memory allocation state's info.
	AllocationState globalAllocationState;
	/// Current Memory allocation state's info.
	AllocationState currentAllocationState;
	/// Last Memory allocation state's info.
	AllocationState lastAllocationState;
};

}// namespace owl::debug

#ifdef OWL_STACKTRACE
#define OWL_SCOPE_TRACE owl::debug::ScopeTrace scopeTrace;
#else
#define OWL_SCOPE_TRACE
#endif

#define OWL_SCOPE_UNTRACK owl::debug::ScopeUntrack scopeUntrack;
