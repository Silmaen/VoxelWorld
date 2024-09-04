/**
 * @file ColorSpace.h
 * @author Silmaen
 * @date 15/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "vectors.h"

namespace owl::math {

/**
 * \brief Convert sRGB color to linear color.
 * \param[in] iColor Input sRGB color.
 * \return Linear color.
 */
constexpr vec4 sRGBToLinear(const vec4& iColor) {
	constexpr auto toLinear = [](const float srgb) {
		return (srgb <= 0.04045f) ? (srgb / 12.92f) : std::pow((srgb + 0.055f) / 1.055f, 2.4f);
	};
	return {toLinear(iColor.r()), toLinear(iColor.g()), toLinear(iColor.b()), iColor.a()};
}

/**
 * \brief Convert linear color to sRgB color.
 * \param[in] iColor Input linear color.
 * \return sRGB colr.
 */
constexpr vec4 linearTosRGB(const vec4& iColor) {
	constexpr auto toSRGB = [](const float iRgb) {
		constexpr float power = 1.0f / 2.4f;
		return iRgb <= 0.0031308f ? 12.92f * iRgb : 1.055f * std::pow(iRgb, power) - 0.055f;
	};
	return {toSRGB(iColor.r()), toSRGB(iColor.g()), toSRGB(iColor.b()), iColor.a()};
}

}// namespace owl::math
