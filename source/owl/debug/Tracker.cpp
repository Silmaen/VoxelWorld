/**
 * @file Tracker.cpp
 * @author Silmaen
 * @date 17/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Tracker.h"

#define OWL_DEALLOC_EXCEPT noexcept

#if !defined(__cpp_sized_deallocation) || __cpp_sized_deallocation == 0
void operator delete(void *memory, size_t size) OWL_DEALLOC_EXCEPT;
#endif

/**
 * @brief Overload of the standard memory allocator
 * @param size Size to allocate
 * @return Pointer to allocated memory
 */
void *operator new(size_t size) {
  owl::debug::Tracker::get().allocate(size);
  return malloc(size);
}

/**
 * @brief Overload of standard memory deallocation
 * @param memory Memory to free
 * @param size Amount to free
 */
void operator delete(void *memory, size_t size) OWL_DEALLOC_EXCEPT {
  owl::debug::Tracker::get().deallocate(size);
  free(memory);
}
/**
 * @brief Overload of standard memory deallocation
 * @param memory Memory to free
 */
void operator delete(void *memory) OWL_DEALLOC_EXCEPT {
  free(memory);
} //---UNCOVER---

namespace owl::debug {

void Tracker::allocate(size_t size) {
  ++currentAllocationState.allocationCalls;
  currentAllocationState.allocatedMemory += size;
  currentAllocationState.memoryPeek =
      std::max(currentAllocationState.memoryPeek,
               currentAllocationState.allocatedMemory);
  ++globalAllocationState.allocationCalls;
  globalAllocationState.allocatedMemory += size;
  globalAllocationState.memoryPeek = std::max(
      globalAllocationState.memoryPeek, globalAllocationState.allocatedMemory);
}
void Tracker::deallocate(size_t size) {
  ++currentAllocationState.deallocationCalls;
  currentAllocationState.allocatedMemory =
      currentAllocationState.allocatedMemory > size
          ? currentAllocationState.allocatedMemory - size
          : 0;
  ++globalAllocationState.deallocationCalls;
  globalAllocationState.allocatedMemory =
      globalAllocationState.allocatedMemory > size
          ? globalAllocationState.allocatedMemory - size
          : 0;
}

const Tracker::AllocationState &Tracker::checkState() {

  lastAllocationState.allocatedMemory = 0;
  lastAllocationState.allocationCalls = 0;
  lastAllocationState.deallocationCalls = 0;
  lastAllocationState.memoryPeek = 0;
  std::swap(currentAllocationState, lastAllocationState);
  return lastAllocationState;
}
const Tracker::AllocationState &Tracker::globals() const {
  return globalAllocationState;
}

} // namespace owl::debug
