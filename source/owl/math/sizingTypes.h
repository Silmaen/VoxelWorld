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
	 * @param[in] iWidth The width.
	 * @param[in] iHeight The height.
	 */
	FrameSize(const uint32_t iWidth, const uint32_t iHeight) : m_width{iWidth}, m_height{iHeight} {}

	/**
	 * @brief Destructor.
	 */
	~FrameSize() = default;

	FrameSize(const FrameSize&) = default;
	FrameSize(FrameSize&&) = default;
	FrameSize& operator=(const FrameSize&) = default;
	FrameSize& operator=(FrameSize&&) = default;

	// ====== COMPARISON ======
	/**
	 * @brief Comparison operator.
	 * @param[in] iOther Other object to compare.
	 * @return True if similar.
	 */
	[[nodiscard]] bool operator==(const FrameSize& iOther) const {
		return (m_width == iOther.m_width) && (m_height == iOther.m_height);
	}

	/**
	 * @brief Comparison operator.
	 * @param[in] iOther Other object to compare.
	 * @return True if not similar.
	 */
	[[nodiscard]] bool operator!=(const FrameSize& iOther) const {
		return (m_width != iOther.m_width) || (m_height != iOther.m_height);
	}

	// ====== ACCESS ======
	/**
	 * @brief Access to the width for reading.
	 * @return The width
	 */
	[[nodiscard]] const uint32_t& getWidth() const { return m_width; }

	/**
	 * @brief Access to the height for reading.
	 * @return The width
	 */
	[[nodiscard]] const uint32_t& getHeight() const { return m_height; }

	/**
	 * @brief Direct access to the width.
	 * @return The width
	 */
	[[nodiscard]] uint32_t& width() { return m_width; }

	/**
	 * @brief Direct access to the height.
	 * @return The width
	 */
	[[nodiscard]] uint32_t& height() { return m_height; }

	/**
	 * @brief Direct access to coordinate.
	 * @param[in] iId Coordinate id.
	 * @return The coordinate.
	 */
	[[nodiscard]] const uint32_t& at(const uint8_t iId) const { return iId == 1 ? m_height : m_width; }
	/**
	 * @brief Direct access to coordinate.
	 * @param[in] iId Coordinate id.
	 * @return The coordinate.
	 */
	[[nodiscard]] uint32_t& at(const uint8_t iId) { return iId == 1 ? m_height : m_width; }

	/**
	 * @brief Direct access to coordinate.
	 * @param[in] iId Coordinate id.
	 * @return The coordinate.
	 */
	[[nodiscard]] uint32_t& operator[](const uint8_t iId) { return at(iId); }

	/**
	 * @brief Direct access to coordinate.
	 * @param[in] iId Coordinate id.
	 * @return The coordinate.
	 */
	[[nodiscard]] const uint32_t& operator[](const uint8_t iId) const { return at(iId); }

	// ====== OPERATORS ======
	/**
	 * @brief Self addition.
	 * @param[in] iOther Other size to add.
	 * @return This object.
	 */
	FrameSize& operator+=(const FrameSize& iOther) {
		m_width += iOther.m_width;
		m_height += iOther.m_height;
		return *this;
	}

	/**
	 * @brief Self subtract.
	 * @param[in] iOther Other size to sub.
	 * @return This object.
	 */
	FrameSize& operator-=(const FrameSize& iOther) {
		m_width -= iOther.m_width;
		m_height -= iOther.m_height;
		return *this;
	}

	/**
	 * @brief Self ratio product.
	 * @param[in] iRatio The scaling ratio.
	 * @return This object.
	 */
	FrameSize& operator*=(double iRatio) {
		iRatio = std::abs(iRatio);
		m_width = static_cast<uint32_t>(std::round(iRatio * m_width));
		m_height = static_cast<uint32_t>(std::round(iRatio * m_height));
		return *this;
	}

	/**
	 * @brief Addition.
	 * @param[in] iOther Other size to add.
	 * @return Result.
	 */
	FrameSize operator+(const FrameSize& iOther) const {
		FrameSize result(*this);
		result += iOther;
		return result;
	}

	/**
	 * @brief Subtract.
	 * @param[in] iOther Other size to sub.
	 * @return Result.
	 */
	FrameSize operator-(const FrameSize& iOther) const {
		FrameSize result(*this);
		result -= iOther;
		return result;
	}

	/**
	 * @brief Ratio product.
	 * @param[in] iRatio The scaling ratio.
	 * @return Result.
	 */
	FrameSize operator*(const double iRatio) const {
		FrameSize result(*this);
		result *= iRatio;
		return result;
	}

	/**
	 * @brief Ratio product
	 * @param[in] iRatio The ratio.
	 * @param[in] iOther The Size object
	 * @return Result.
	 */
	friend FrameSize operator*(const double iRatio, const FrameSize& iOther) { return iOther * iRatio; }
	// ===== COMPUTATIONS =====
	/**
	 * @brief Compute the surface.
	 * @return The surface.
	 */
	[[nodiscard]] uint32_t surface() const { return m_width * m_height; }

private:
	/// Frame width.
	uint32_t m_width = 0;
	/// Frame height.
	uint32_t m_height = 0;
};
} // namespace owl::math
