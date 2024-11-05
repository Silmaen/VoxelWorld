/**
 * @file SoundData.h
 * @author Silmaen
 * @date 11/5/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "sound/SoundData.h"

namespace owl::sound::openal {

/**
 * @brief Class representing what's required for playing sound.
 */
class OWL_API SoundData final : public sound::SoundData {
public:
	/**
	 * @brief Default constructor.
	 */
	SoundData() = default;
	/**
	 * @brief Default destructor.
	 */
	~SoundData() override;
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
	 * @brief Initialize the sound data.
	 */
	void init() override;

private:
};

}// namespace owl::sound::openal
