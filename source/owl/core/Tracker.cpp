/**
 * @file Tracker.cpp
 * @author Silmaen
 * @date 17/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Tracker.h"
#include <memory>

/**
 * @brief Overload of the standard memory allocator
 * @param size Size to allocate
 * @return Pointer to allocated memory
 */
void *operator new(size_t size) {
  owl::core::Tracker::get().allocate(size);
  return malloc(size);
}

/**
 * @brief Overload of standard memory deallocation
 * @param memory Memory to free
 * @param size Amount to free
 */
void operator delete(void *memory, size_t size) {
  owl::core::Tracker::get().deallocate(size);
  free(memory);
}
/**
 * @brief Overload of standard memory deallocation
 * @param memory Memory to free
 */
void operator delete(void *memory) { free(memory); }//---UNCOVER---

namespace owl::core {

void Tracker::allocate(size_t size) {
  ++m_currentAllocationState.m_allocationCalls;
  m_currentAllocationState.m_allocatedMemory += size;
  m_currentAllocationState.m_memoryPeek =
      std::max(m_currentAllocationState.m_memoryPeek,
               m_currentAllocationState.m_allocatedMemory);
  ++m_globalAllocationState.m_allocationCalls;
  m_globalAllocationState.m_allocatedMemory += size;
  m_globalAllocationState.m_memoryPeek =
      std::max(m_globalAllocationState.m_memoryPeek,
               m_globalAllocationState.m_allocatedMemory);
}
void Tracker::deallocate(size_t size) {
  ++m_currentAllocationState.m_deallocationCalls;
  m_currentAllocationState.m_allocatedMemory = m_currentAllocationState.m_allocatedMemory > size? m_currentAllocationState.m_allocatedMemory-size:0;
  ++m_globalAllocationState.m_deallocationCalls;
  m_globalAllocationState.m_allocatedMemory = m_globalAllocationState.m_allocatedMemory > size? m_globalAllocationState.m_allocatedMemory-size:0;
}

const Tracker::AllocationState &Tracker::checkState() {

  m_lastAllocationState.m_allocatedMemory = 0;
  m_lastAllocationState.m_allocationCalls = 0;
  m_lastAllocationState.m_deallocationCalls = 0;
  m_lastAllocationState.m_memoryPeek = 0;
  std::swap(m_currentAllocationState, m_lastAllocationState);
  return m_lastAllocationState;
}
const Tracker::AllocationState &Tracker::globals() const {
  return m_globalAllocationState;
}

} // namespace owl::core
