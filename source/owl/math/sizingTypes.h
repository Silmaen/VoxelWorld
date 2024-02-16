/**
 * @file sizingTypes.h
 * @author Silmaen
 * @date 26/12/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

namespace owl::math {

/**
 * @brief Class to store a frame size.
 */
class OWL_API FrameSize final {
public:
	/**
	 * @brief Default constructor.
	 */
	FrameSize();

	/**
	 * @brief Constructor with values.
	 * @param w The width.
	 * @param h The height.
	 */
	FrameSize(uint32_t w, uint32_t h) : _width{w}, _height{h} {}

	/**
	 * @brief Destructor.
	 */
	virtual ~FrameSize();

	FrameSize(const FrameSize &) = default;
	FrameSize(FrameSize &&) = default;
	FrameSize &operator=(const FrameSize &) = default;
	FrameSize &operator=(FrameSize &&) = default;

	// ====== COMPARISON ======
	/**
	 * @brief Comparison operator.
	 * @param other Other object to compare.
	 * @return True if similar.
	 */
	[[nodiscard]] bool operator==(const FrameSize &other) const {
		return (_width == other._width) && (_height == other._height);
	}

	/**
	 * @brief Comparison operator.
	 * @param other Other object to compare.
	 * @return True if not similar.
	 */
	[[nodiscard]] bool operator!=(const FrameSize &other) const {
		return (_width != other._width) || (_height != other._height);
	}

	// ====== ACCESS ======
	/**
	 * @brief Access to the width for reading.
	 * @return The width
	 */
	[[nodiscard]] const uint32_t &getWidth() const { return _width; }

	/**
	 * @brief Access to the height for reading.
	 * @return The width
	 */
	[[nodiscard]] const uint32_t &getHeight() const { return _height; }

	/**
	 * @brief Direct access to the width.
	 * @return The width
	 */
	[[nodiscard]] uint32_t &width() { return _width; }

	/**
	 * @brief Direct access to the height.
	 * @return The width
	 */
	[[nodiscard]] uint32_t &height() { return _height; }

	/**
	 * @brief Direct access to coordinate.
	 * @param id Coordinate id.
	 * @return The coordinate.
	 */
	[[nodiscard]] const uint32_t &at(uint8_t id) const { return id == 1 ? _height : _width; }
	/**
	 * @brief Direct access to coordinate.
	 * @param id Coordinate id.
	 * @return The coordinate.
	 */
	[[nodiscard]] uint32_t &at(uint8_t id) { return id == 1 ? _height : _width; }

	/**
	 * @brief Direct access to coordinate.
	 * @param id Coordinate id.
	 * @return The coordinate.
	 */
	[[nodiscard]] uint32_t &operator[](uint8_t id) { return at(id); }

	/**
	 * @brief Direct access to coordinate.
	 * @param id Coordinate id.
	 * @return The coordinate.
	 */
	[[nodiscard]] const uint32_t &operator[](uint8_t id) const { return at(id); }

	// ====== OPERATORS ======
	/**
	 * @brief Self addition.
	 * @param other Other size to add.
	 * @return This object.
	 */
	FrameSize &operator+=(const FrameSize &other) {
		_width += other._width;
		_height += other._height;
		return *this;
	}

	/**
	 * @brief Self subtract.
	 * @param other Other size to sub.
	 * @return This object.
	 */
	FrameSize &operator-=(const FrameSize &other) {
		_width -= other._width;
		_height -= other._height;
		return *this;
	}

	/**
	 * @brief Self ratio product.
	 * @param ratio The scaling ratio.
	 * @return This object.
	 */
	FrameSize &operator*=(double ratio) {
		ratio = std::abs(ratio);
		_width = static_cast<uint32_t>(std::round(ratio * _width));
		_height = static_cast<uint32_t>(std::round(ratio * _height));
		return *this;
	}

	/**
	 * @brief Addition.
	 * @param other Other size to add.
	 * @return Result.
	 */
	FrameSize operator+(const FrameSize &other) const {
		FrameSize result(*this);
		result += other;
		return result;
	}

	/**
	 * @brief Subtract.
	 * @param other Other size to sub.
	 * @return Result.
	 */
	FrameSize operator-(const FrameSize &other) const {
		FrameSize result(*this);
		result -= other;
		return result;
	}

	/**
	 * @brief Ratio product.
	 * @param ratio The scaling ratio.
	 * @return Result.
	 */
	FrameSize operator*(double ratio) const {
		FrameSize result(*this);
		result *= ratio;
		return result;
	}

	/**
	 * @brief Ratio product
	 * @param ratio The ratio.
	 * @param other The Size object
	 * @return Result.
	 */
	friend FrameSize operator*(double ratio, const FrameSize &other) {
		return other * ratio;
	}
	// ===== COMPUTATIONS =====
	/**
	 * @brief Compute the surface.
	 * @return The surface.
	 */
	uint32_t surface() const {
		return _width * _height;
	}

private:
	/// Frame width.
	uint32_t _width = 0;
	/// Frame height.
	uint32_t _height = 0;
};

}// namespace owl::math
