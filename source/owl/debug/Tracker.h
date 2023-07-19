/**
 * @file Tracker.h
 * @author Silmaen
 * @date 17/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

using size_t = std::size_t;

/**
 * @namespace owl
 * @brief Base namespace for the project.
 */

/**
 * @brief namespace for core functions.
 */
namespace owl::debug {
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
	~Tracker() = default;

	/**
	 * @brief Get engine instance.
	 * @return The engine instance.
	 */
	static Tracker &get();

	/**
	 * @brief Function called at each allocation.
	 * @param size The Allocated size.
	 */
	void allocate(size_t size);

	/**
	 * @brief Function called each de-allocation.
	 * @param size De-allocation size.
	 */
	void deallocate(size_t size);

	/**
	 * @brief Result structure of allocation state.
	 */
	struct AllocationState {
		size_t allocatedMemory = 0;  ///< Amount of allocated memory.
		size_t allocationCalls = 0;  ///< Amount of memory allocation calls.
		size_t deallocationCalls = 0;///< Amount of de-allocation calls.
		size_t memoryPeek = 0;       ///< Max seen amount of memory.
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
	Tracker() = default;

	/// Global Memory allocation state's info.
	AllocationState globalAllocationState;
	/// Current Memory allocation state's info.
	AllocationState currentAllocationState;
	/// Last Memory allocation state's info.
	AllocationState lastAllocationState;
};

}// namespace owl::debug
