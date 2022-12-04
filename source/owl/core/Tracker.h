/**
 * @file Tracker.h
 * @author Silmaen
 * @date 17/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <cstdint>

using size_t=std::size_t;

/**
 * @namespace owl
 * @brief Base namespace for the project
 */
/**
 * @brief namespace for core functions
 */
namespace owl::core {
/**
 * @brief Class Tracker
 *
 * A very simple memory allocation tracker
 */
class Tracker {
public:
  Tracker(const Tracker &) = delete;
  Tracker(Tracker &&) = delete;
  Tracker &operator=(const Tracker &) = delete;
  Tracker &operator=(Tracker &&) = delete;
  /**
   * @brief Destructor.
   */
  virtual ~Tracker() = default; //---UNCOVER---
  /**
   * @brief Get engine instance
   * @return The engine instance
   */
  static Tracker &get() {
    static Tracker instance;
    return instance;
  }

  /**
   * @brief Function called at each allocation
   * @param size The Allocated size
   */
  void allocate(size_t size);

  /**
   * @brief Function called each deallocation
   * @param size Deallocation size
   */
  void deallocate(size_t size);

  /**
   * @brief Result structure of allocation state
   */
  struct AllocationState {
    size_t m_allocatedMemory = 0;   ///< Amount of allocated memory
    size_t m_allocationCalls = 0;   ///< Amount of memory allocation calls
    size_t m_deallocationCalls = 0; ///< Amount of deallocation calls
    size_t m_memoryPeek = 0;        ///< Max seen amount of memory
  };

  /**
   * @brief Reset current memory state monitor and give the previous status
   * @return Status since last call to check
   */
  const AllocationState &checkState();
  /**
   * @brief Get the memory state since the start of the program
   * @return Memory state
   */
  [[nodiscard]] const AllocationState &globals() const;

private:
  /**
   * @brief Default constructor.
   */
  Tracker() = default;

  AllocationState m_globalAllocationState;
  AllocationState m_currentAllocationState;
  AllocationState m_lastAllocationState;
};

} // namespace owl::core
