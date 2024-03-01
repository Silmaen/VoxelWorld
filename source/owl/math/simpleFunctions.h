/**
 * @file simpleFunctions.h
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

/**
 * @brief Namespace for mathematical functions.
 */
namespace owl::math {
/**
 * @brief Simple clamp function with no checks.
 * @tparam Data Data's type to clamp (must be order-able).
 * @param[in] iValue Input value.
 * @param[in] iMinValue min value.
 * @param[in] iMaxValue max value.
 * @return Clamped value.
 */
template <typename Data>
constexpr Data clamp(const Data& iValue, const Data& iMinValue,
                     const Data& iMaxValue) {
 return iValue < iMinValue ? iMinValue : iValue > iMaxValue ? iMaxValue : iValue;
}

/**
 * @brief Clamp function with mix/max ordering.
 * @tparam Data Data's type to clamp (must be order-able).
 * @param[in] iValue Input value.
 * @param[in] iMinValue min value.
 * @param[in] iMaxValue max value.
 * @return Clamped value.
 */
template <typename Data>
constexpr Data clampSafe(const Data& iValue, Data iMinValue, Data iMaxValue) {
 if (iMaxValue < iMinValue)
  std::swap(iMinValue, iMaxValue);
 return iValue < iMinValue ? iMinValue : iValue > iMaxValue ? iMaxValue : iValue;
}

/**
 * @brief Heaviside function.
 * @tparam Data Data's type.
 * @param[in] iValue Value to check.
 * @return 1 if input positive, else return 0.
 */
template <typename Data>
constexpr Data heaviside(const Data& iValue) { return iValue > 0 ? Data{1} : Data{}; }

/**
 * @brief Sign function.
 * @tparam Data Data's type.
 * @param[in] iValue Value to check.
 * @return 0 if input is 0 1 if input positive, else return -1.
 */
template <typename Data>
constexpr Data sign(const Data& iValue) { return iValue > 0 ? Data{1} : (iValue < 0 ? Data{-1} : 0); }
} // namespace owl::math
