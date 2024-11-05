/**
 * @file SoundData.h
 * @author Silmaen
 * @date 11/5/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

namespace owl::sound {

/**
 * @brief Abstract class representing what's required for playing sound.
 */
class OWL_API SoundData {
public:
	/**
	 * @brief Default constructor.
	 */
	SoundData() = default;
	/**
	 * @brief Default destructor.
	 */
	virtual ~SoundData();
	/**
	 * @brief Default copy constructor.
	 */
	SoundData(const SoundData&) = default;
	/**
	 * @brief Default move constructor.
	 */
	SoundData(SoundData&&) = default;
	/**
	 * @brief Default copy affectation operator.
	 */
	auto operator=(const SoundData&) -> SoundData& = default;
	/**
	 * @brief Default move affectation operator.
	 */
	auto operator=(SoundData&&) -> SoundData& = default;
	/**
	 * @brief Create a new drax data buffer.
	 * @return Pointer to the created buffer.
	 */
	static auto create() -> shared<SoundData>;

	/**
	 * @brief Initialize the sound data.
	 */
	virtual void init() = 0;

private:
};

}// namespace owl::sound
