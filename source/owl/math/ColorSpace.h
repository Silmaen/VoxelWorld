/**
 * @file ColorSpace.h
 * @author Silmaen
 * @date 15/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

namespace owl::math {

constexpr glm::vec4 sRGBToLinear(const glm::vec4 &color) {
	constexpr auto toLinear = [](float srgb) {
		return (srgb <= 0.04045f) ? (srgb / 12.92f) : std::pow((srgb + 0.055f) / 1.055f, 2.4f);
	};
	return {toLinear(color.r), toLinear(color.g), toLinear(color.b), color.a};
}

constexpr glm::vec4 linearTosRGB(const glm::vec4 &color) {
	constexpr auto toSRGB = [](float rgb) {
		constexpr float power = 1.0f / 2.4f;
		return rgb <= 0.0031308f ? 12.92f * rgb : 1.055f * std::pow(rgb, power) - 0.055f;
	};
	return {toSRGB(color.r), toSRGB(color.g), toSRGB(color.b), color.a};
}

}// namespace owl::math
