/**
 * @file Device.cpp
 * @author Silmaen
 * @date 03/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Device.h"
#include "math/simpleFunctions.h"
#include <stb_image.h>

namespace owl::input::video {

namespace {

void convertNv12ToRgb24(const uint8_t *iNv12Buffer, const math::FrameSize &iFrameSize, uint8_t *oRgb24Buffer) {
	// Chaque composant Y occupe width * height octets
	const uint32_t ySize = iFrameSize.surface();

	// Pointeurs vers les composants Y, U et V dans le tampon NV12
	const uint8_t *yComponent = iNv12Buffer;
	const uint8_t *uvComponent = iNv12Buffer + ySize;

	// Parcourir chaque ligne
	for (uint32_t i = 0; i < iFrameSize.getHeight(); i++) {
		// Parcourir chaque pixel de la ligne
		for (uint32_t j = 0; j < iFrameSize.getWidth(); j++) {
			// Indices dans le tampon NV12
			const uint32_t yIndex = i * iFrameSize.getWidth() + j;
			const uint32_t uvIndex = i / 2 * iFrameSize.getWidth() + (j & ~1ull);
			// Indices dans le tampon RGB24
			const uint32_t rgbIndex = ((i + 1) * iFrameSize.getWidth() - j - 1) * 3;
			// Conversion YUV vers RGB
			const int32_t c = yComponent[yIndex] - 16;
			const int32_t d = uvComponent[uvIndex] - 128;
			const int32_t e = uvComponent[uvIndex + 1] - 128;
			// Calcul des composantes RGB, Limiter les valeurs à l'intervalle [0, 255]
			oRgb24Buffer[rgbIndex] = static_cast<uint8_t>(math::clamp((298 * c + 409 * e + 128) >> 8, 0, 255));
			oRgb24Buffer[rgbIndex + 1] = static_cast<uint8_t>(math::clamp(
					(298 * c - 100 * d - 208 * e + 128) >> 8, 0, 255));
			oRgb24Buffer[rgbIndex + 2] = static_cast<uint8_t>(math::clamp((298 * c + 516 * d + 128) >> 8, 0, 255));
		}
	}
}

void convertYuYvToRgb24(const uint8_t *iYuYvBuffer, const math::FrameSize &iFrameSize, uint8_t *oRgb24Buffer) {
	// Chaque composant YUV prend deux octets dans le format YUYV
	const uint32_t yuyvSize = iFrameSize.surface() * 2;

	// Parcourir chaque paire de pixels YUYV
	for (uint32_t i = 0; i < yuyvSize; i += 4) {
		// Conversion YUV vers RGB pour le premier pixel
		const int32_t c0 = iYuYvBuffer[i] - 16;
		const int32_t d0 = iYuYvBuffer[i + 1] - 128;
		const int32_t e0 = iYuYvBuffer[i + 3] - 128;
		// Conversion YUV vers RGB pour le deuxième pixel
		const int32_t c1 = iYuYvBuffer[i + 2] - 16;
		// Indices dans le tampon RGB24
		const uint32_t rgbIndex = i * 3 / 2;

		// Stocker les composantes RGB dans le tampon RGB24 pour les deux pixels
		oRgb24Buffer[rgbIndex] = static_cast<uint8_t>(math::clamp((298 * c0 + 409 * e0 + 128) >> 8, 0, 255));
		oRgb24Buffer[rgbIndex + 1] = static_cast<uint8_t>(math::clamp((298 * c0 - 100 * d0 - 208 * e0 + 128) >> 8, 0,
		                                                              255));
		oRgb24Buffer[rgbIndex + 2] = static_cast<uint8_t>(math::clamp((298 * c0 + 516 * d0 + 128) >> 8, 0, 255));

		oRgb24Buffer[rgbIndex + 3] = static_cast<uint8_t>(math::clamp((298 * c1 + 409 * e0 + 128) >> 8, 0, 255));
		oRgb24Buffer[rgbIndex + 4] = static_cast<uint8_t>(math::clamp((298 * c1 - 100 * d0 - 208 * e0 + 128) >> 8, 0,
		                                                              255));
		oRgb24Buffer[rgbIndex + 5] = static_cast<uint8_t>(math::clamp((298 * c1 + 516 * d0 + 128) >> 8, 0, 255));
	}
}

void convertMJpegToRgb24(const uint8_t *iJpegBuffer, const int32_t iJpegSize, const math::FrameSize &iFrameSize,
                         uint8_t *oRgb24Buffer) {
	int comp, width, height;
	stbi_set_flip_vertically_on_load(0);
	uint8_t *buffer = stbi_load_from_memory(iJpegBuffer, iJpegSize, &width, &height, &comp, 3);
	if (buffer == nullptr) {
		OWL_CORE_WARN("Jpeg decoding: nullptr result.")
		return;
	}
	if (iFrameSize != math::FrameSize{static_cast<uint32_t>(width), static_cast<uint32_t>(height)}) {
		OWL_CORE_WARN("Jpeg decoding: size missmatch ({} {}) expecting {} {}.", width, height, iFrameSize.getWidth(),
		              iFrameSize.getHeight())
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
			const int sourceIndex = (i * width + j) * 3;
			const int destinationIndex = ((i + 1) * width - j - 1) * 3;// Inversion horizontale
			std::memcpy(oRgb24Buffer + destinationIndex, buffer + sourceIndex, 3);
		}
	}
	//std::memcpy(rgb24Buffer, buffer, frameSize.surface() * 3);
	stbi_image_free(buffer);
}

}// namespace

Device::Device(std::string iName) : m_name(std::move(iName)) {}

Device::~Device() = default;

std::vector<uint8_t> Device::getRgbBuffer(const uint8_t *iInputBuffer, const int32_t iBufferSize) const {
	std::vector<uint8_t> output;
	if (m_pixFormat == PixelFormat::Nv12) {
		output.resize(3 * m_size.surface());
		convertNv12ToRgb24(iInputBuffer, m_size, output.data());
	} else if (m_pixFormat == PixelFormat::Rgb24) {
		output.resize(3 * m_size.surface());
		memcpy(output.data(), iInputBuffer, output.size());
	} else if (m_pixFormat == PixelFormat::YuYv) {
		output.resize(3 * m_size.surface());
		convertYuYvToRgb24(iInputBuffer, m_size, output.data());
	} else if (m_pixFormat == PixelFormat::MJpeg) {
		output.resize(3 * m_size.surface());
		convertMJpegToRgb24(iInputBuffer, iBufferSize, m_size, output.data());
	} else { OWL_CORE_WARN("Unkown or unsupported pixel format, empty output buffer.") }
	return output;
}

bool Device::isPixelFormatSupported(const PixelFormat &iPixFormat) {
	switch (iPixFormat) {
		case PixelFormat::Rgb24:
		case PixelFormat::YuYv:
		case PixelFormat::Nv12:
		case PixelFormat::MJpeg:
			return true;
		case PixelFormat::Unknwon:
			return false;
	}
	return false;
}

}// namespace owl::input::video
