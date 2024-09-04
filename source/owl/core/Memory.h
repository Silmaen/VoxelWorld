/**
 * @file Memory.h
 * @author Silmaen
 * @date 04/09/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <memory>

/**
 * @brief Base Namespace for the project.
 */
namespace owl {

/// Wrap to unique pointer.
template<typename T>
using uniq = std::unique_ptr<T>;

/**
 * @brief Wrap to unique pointer creator.
 * @tparam T Type of data.
 * @tparam Args Args type to pass to the constructor.
 * @param[in] iArgs Args to pass to the constructor.
 * @return Unique pointer.
 */
template<typename T, typename... Args>
constexpr auto mkUniq(Args&&... iArgs) -> uniq<T> {
	return std::make_unique<T>(std::forward<Args>(iArgs)...);
}

/// Wrap to shared pointer.
template<typename T>
using shared = std::shared_ptr<T>;

/**
 * @brief Wrap to shared pointer creator.
 * @tparam T Type of data.
 * @tparam Args Args type to pass to the constructor.
 * @param[in] iArgs Args to pass to the constructor.
 * @return Shared pointer.
 */
template<typename T, typename... Args>
constexpr auto mkShared(Args&&... iArgs) -> shared<T> {
	return std::make_shared<T>(std::forward<Args>(iArgs)...);
}

}// namespace owl
