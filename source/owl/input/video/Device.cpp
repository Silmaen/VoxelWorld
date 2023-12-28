/**
 * @file Device.cpp
 * @author Silmaen
 * @date 03/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Device.h"
#include "math/simpleFunctions.h"
#include <stb_image.h>

namespace owl::input::video {

namespace {

void ConvertNV12ToRGB24(const uint8_t *nv12Buffer, const math::FrameSize &frameSize, uint8_t *rgb24Buffer) {
	// Chaque composant Y occupe width * height octets
	const uint32_t ySize = frameSize.surface();

	// Pointeurs vers les composants Y, U et V dans le tampon NV12
	const uint8_t *yComponent = nv12Buffer;
	const uint8_t *uvComponent = nv12Buffer + ySize;

	// Parcourir chaque ligne
	for (uint32_t i = 0; i < frameSize.getHeight(); i++) {
		// Parcourir chaque pixel de la ligne
		for (uint32_t j = 0; j < frameSize.getWidth(); j++) {
			// Indices dans le tampon NV12
			const uint32_t yIndex = i * frameSize.getWidth() + j;
			const uint32_t uvIndex = (i / 2) * frameSize.getWidth() + (j & ~1ull);
			// Indices dans le tampon RGB24
			const uint32_t rgbIndex = (i * frameSize.getWidth() + j) * 3;
			// Conversion YUV vers RGB
			const int32_t c = yComponent[yIndex] - 16;
			const int32_t d = uvComponent[uvIndex] - 128;
			const int32_t e = uvComponent[uvIndex + 1] - 128;
			// Calcul des composantes RGB, Limiter les valeurs à l'intervalle [0, 255]
			rgb24Buffer[rgbIndex] = static_cast<uint8_t>(math::clamp((298 * c + 409 * e + 128) >> 8, 0, 255));
			rgb24Buffer[rgbIndex + 1] = static_cast<uint8_t>(math::clamp((298 * c - 100 * d - 208 * e + 128) >> 8, 0, 255));
			rgb24Buffer[rgbIndex + 2] = static_cast<uint8_t>(math::clamp((298 * c + 516 * d + 128) >> 8, 0, 255));
		}
	}
}

void ConvertYUYVToRGB24(const uint8_t *yuyvBuffer, const math::FrameSize &frameSize, uint8_t *rgb24Buffer) {
	// Chaque composant YUV prend deux octets dans le format YUYV
	const uint32_t yuyvSize = frameSize.surface() * 2;

	// Parcourir chaque paire de pixels YUYV
	for (uint32_t i = 0; i < yuyvSize; i += 4) {
		// Conversion YUV vers RGB pour le premier pixel
		const int32_t C0 = yuyvBuffer[i] - 16;
		const int32_t D0 = yuyvBuffer[i + 1] - 128;
		const int32_t E0 = yuyvBuffer[i + 3] - 128;
		// Conversion YUV vers RGB pour le deuxième pixel
		const int32_t C1 = yuyvBuffer[i + 2] - 16;
		// Indices dans le tampon RGB24
		const uint32_t rgbIndex = i * 3 / 2;

		// Stocker les composantes RGB dans le tampon RGB24 pour les deux pixels
		rgb24Buffer[rgbIndex] = static_cast<uint8_t>(math::clamp((298 * C0 + 409 * E0 + 128) >> 8, 0, 255));
		rgb24Buffer[rgbIndex + 1] = static_cast<uint8_t>(math::clamp((298 * C0 - 100 * D0 - 208 * E0 + 128) >> 8, 0, 255));
		rgb24Buffer[rgbIndex + 2] = static_cast<uint8_t>(math::clamp((298 * C0 + 516 * D0 + 128) >> 8, 0, 255));

		rgb24Buffer[rgbIndex + 3] = static_cast<uint8_t>(math::clamp((298 * C1 + 409 * E0 + 128) >> 8, 0, 255));
		rgb24Buffer[rgbIndex + 4] = static_cast<uint8_t>(math::clamp((298 * C1 - 100 * D0 - 208 * E0 + 128) >> 8, 0, 255));
		rgb24Buffer[rgbIndex + 5] = static_cast<uint8_t>(math::clamp((298 * C1 + 516 * D0 + 128) >> 8, 0, 255));
	}
}

void ConvertMJPEGToRGB24(const uint8_t *jpegBuffer, const int32_t jpegSize, const math::FrameSize &frameSize, uint8_t *rgb24Buffer) {
	int comp, width, height;
	stbi_set_flip_vertically_on_load(0);
	uint8_t *buffer = stbi_load_from_memory(jpegBuffer, jpegSize, &width, &height, &comp, 3);
	if (buffer == nullptr) {
		OWL_CORE_WARN("Jpeg decoding: nullptr result.")
		return;
	}
	if (frameSize != math::FrameSize{static_cast<uint32_t>(width), static_cast<uint32_t>(height)}) {
		OWL_CORE_WARN("Jpeg decoding: size missmatch ({} {}) expecting {} {}.", width, height, frameSize.getWidth(), frameSize.getHeight())
		stbi_image_free(buffer);
		return;
	}
	if (comp != 3) {
		OWL_CORE_WARN("Jpeg decoding: wrong number of channels: {} expecting 3.", comp)
		stbi_image_free(buffer);
		return;
	}
	//int rowSize = width * 3;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int sourceIndex = (i * width + j) * 3;
			int destinationIndex = ((i + 1) * width - j - 1) * 3;// Inversion horizontale
			std::memcpy(rgb24Buffer + destinationIndex, buffer + sourceIndex, 3);
		}
	}
	//std::memcpy(rgb24Buffer, buffer, frameSize.surface() * 3);
	stbi_image_free(buffer);
}

}// namespace

Device::Device(std::string _name) : name(std::move(_name)) {}

Device::~Device() = default;

std::vector<uint8_t> Device::getRGBBuffer(const uint8_t *inputBuffer, const int32_t bufferSize) const {
	std::vector<uint8_t> output;
	if (pixFormat == PixelFormat::NV12) {
		output.resize(3 * size.surface());
		ConvertNV12ToRGB24(inputBuffer, size, output.data());
	} else if (pixFormat == PixelFormat::RGB24) {
		output.resize(3 * size.surface());
		memcpy(output.data(), inputBuffer, output.size());
	} else if (pixFormat == PixelFormat::YUYV) {
		output.resize(3 * size.surface());
		ConvertYUYVToRGB24(inputBuffer, size, output.data());
	} else if (pixFormat == PixelFormat::MJPEG) {
		output.resize(3 * size.surface());
		ConvertMJPEGToRGB24(inputBuffer, bufferSize, size, output.data());
	} else {
		OWL_CORE_WARN("Unkown or unsupported pixel format, empty output buffer.")
	}
	return output;
}

bool Device::isPixelFormatSupported(const PixelFormat &pixFormat) {
	switch (pixFormat) {
		case PixelFormat::RGB24:
		case PixelFormat::YUYV:
		case PixelFormat::NV12:
		case PixelFormat::MJPEG:
			return true;
		case PixelFormat::Unknwon:
		default:
			return false;
	}
}

}// namespace owl::input::video
