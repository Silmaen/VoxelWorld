/**
 * @file SoundLibrary.h
 * @author Silmaen
 * @date 1/8/25
 * Copyright © 2025 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "SoundData.h"
#include "core/assets/AssetLibrary.h"

namespace owl::core::assets {

/**
 * @brief Specialization of Asset for sound data.
 */
template<>
class Asset<sound::SoundData> {
public:
	static auto extensions() -> std::vector<std::string> { return {".wav"}; }
};

}// namespace owl::core::assets

namespace owl::sound {


/**
 * @brief Class managing sound assets.
 */
class SoundLibrary final : public core::assets::AssetLibrary<SoundData> {
public:
	/**
	 * @brief Default constructor.
	 */
	SoundLibrary();

	/**
	 * @brief Default destructor.
	 */
	~SoundLibrary() override;

	/**
	 * @brief Default copy constructor.
	 */
	SoundLibrary(const SoundLibrary&) = default;

	/**
	 * @brief Default move constructor.
	 */
	SoundLibrary(SoundLibrary&&) = default;

	/**
	 * @brief Default copy affectation operator.
	 */
	auto operator=(const SoundLibrary&) -> SoundLibrary& = default;

	/**
	 * @brief Default move affectation operator.
	 */
	auto operator=(SoundLibrary&&) -> SoundLibrary& = default;

private:
};

}// namespace owl::sound
