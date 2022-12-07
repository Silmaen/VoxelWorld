/**
 * @file TrakerClient.h
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 *
 * This file should be included only once in the client side.
 * This allow the memory TRacker to track memory allocation from both engine &
 * client in the case of shared library build.
 */
#pragma once

#ifdef OWL_BUILD_SHARED
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
#endif
