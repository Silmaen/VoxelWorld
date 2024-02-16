/**
 * @file UUID.h
 * @author Silmaen
 * @date 14/01/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

namespace owl::core {

/**
 * @brief Uuid object.
 */
class OWL_API UUID {
public:
	UUID(const UUID &) = default;
	UUID(UUID &&) = default;
	UUID &operator=(const UUID &) = default;
	UUID &operator=(UUID &&) = default;

	/**
	 * @brief Default constructor.
	 */
	UUID();

	/**
	 * @brief Default constructor.
	 * @param Uuid The uuid to initialize.
	 */
	UUID(uint64_t Uuid) : uuid{Uuid} {}

	/**
	 * @brief Destructor.
	 */
	~UUID() = default;

	/**
	 * @brief Implicit conversion to uint64_t.
	 * @return Access const to internal ID.
	 */
	operator uint64_t() const { return uuid; }

private:
	/// The uuid code.
	uint64_t uuid;
};

}// namespace owl::core

/// @cond

namespace std {

/**
 * @brief Hash function for the UUID object.
 */
template<>
struct hash<owl::core::UUID> {
	std::size_t operator()(const owl::core::UUID &uuid) const {
		return hash<uint64_t>()(static_cast<uint64_t>(uuid));
	}
};

}// namespace std

/// @endcond
