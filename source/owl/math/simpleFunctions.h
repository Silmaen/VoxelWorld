/**
 * @file simpleFunctions.h
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

/**
 * @brief Namespace for mathematical functions
 */
namespace owl::math {

/**
 * @brief Simple clamp function with no checks
 * @tparam data Data's type to clamp (must be order-able)
 * @param input Input value
 * @param minValue min value
 * @param maxValue max value
 * @return Clamped value
 */
template <typename data>
constexpr data clamp(const data &input, const data &minValue,
                     const data &maxValue) {
  return input < minValue ? minValue : input > maxValue ? maxValue : input;
}

/**
 * @brief Clamp function with mix/max ordering
 * @tparam data Data's type to clamp (must be order-able)
 * @param input Input value
 * @param minValue min value
 * @param maxValue max value
 * @return Clamped value
 */
template <typename data>
constexpr data clampSafe(const data &input, data minValue, data maxValue) {
  if (maxValue < minValue)
    std::swap(minValue, maxValue);
  return input < minValue ? minValue : input > maxValue ? maxValue : input;
}

/**
 * @brief Heaviside function
 * @tparam data Data's type
 * @param input Value to check
 * @return 1 if input positive, else return 0
 */
template <typename data> constexpr data heaviside(const data &input) {
  return input > 0 ? data{1} : data{};
}

/**
 * @brief Sign function
 * @tparam data Data's type
 * @param input Value to check
 * @return 0 if input is 0 1 if input positive, else return -1
 */
template <typename data> constexpr data sign(const data &input) {
  return input > 0 ? data{1} : (input < 0 ? data{-1} : 0);
}

} // namespace owl::math
