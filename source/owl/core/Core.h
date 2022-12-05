/**
 * @file Application.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <memory>

#ifdef OWL_PLATFORM_WINDOWS
#ifdef OWL_BUILD_SHARED
#ifdef OWL_BUILD_DLL
#define OWL_API __declspec(dllexport)
#else
#define OWL_API __declspec(dllimport)
#endif
#else
#define OWL_API
#endif
#else
#define OWL_API
#endif

#ifdef OWL_DEBUG
#define OWL_ENABLE_ASSERTS
#endif

#ifdef OWL_ENABLE_ASSERTS
#include <debugbreak.h>
#define OWL_ASSERT(x, ...) {if(!(x)) {OWL_ERROR("Assertion Failed: {}", __VA_ARGS__); debug_break();}}
#define OWL_CORE_ASSERT(x, ...) {if(!(x)) {OWL_CORE_ERROR("Assertion Failed: {}", __VA_ARGS__); debug_break();}}
#else
#define OWL_ASSERT(x, ...)
#define OWL_CORE_ARREST(x, ...)
#endif


/**
 * @brief Base Namespace for the project
 */
namespace owl {

/// Wrap to unique pointer
template <typename T> using uniq = std::unique_ptr<T>;
/**
 * @brief  Wrap to unique pointer creator
 * @tparam T Type of data
 * @tparam Args Args type to pass to the constructor
 * @param args Args to pass to the constructor
 * @return Unique pointer
 */
template <typename T, typename... Args>
constexpr uniq<T> mk_uniq(Args &&...args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

/// Wrap to shared pointer
template <typename T> using shrd = std::shared_ptr<T>;
/**
 * @brief Wrap to shared pointer creator
* @tparam T Type of data
* @tparam Args Args type to pass to the constructor
* @param args Args to pass to the constructor
* @return Shared pointer
*/
template <typename T, typename... Args>
constexpr shrd<T> mk_shrd(Args &&...args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace owl
