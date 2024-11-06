/**
 * @file SoundSystem.h
 * @author Silmaen
 * @date 11/5/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "SoundCommand.h"

namespace owl::sound {

/**
 * @brief Class SoundSystem.
 */
class OWL_API SoundSystem {
public:
	/**
	 * @brief Default constructor.
	 */
	SoundSystem() = default;
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem(SoundSystem&&) = delete;
	auto operator=(const SoundSystem&) -> SoundSystem& = delete;
	auto operator=(SoundSystem&&) -> SoundSystem& = delete;
	/**
	 * @brief Default destructor.
	 */
	~SoundSystem() = default;
	/**
		 * @brief Initialize the renderer.
		 */
	static void init();

	/**
	 * @brief Stops the renderer.
	 */
	static void shutdown();

	/**
	 * @brief Reset the renderer.
	 */
	static void reset();

	/**
	 * @brief The state of the renderer.
	 */
	enum struct State : uint8_t {
		Created,///< Renderer just created.
		Running,///< Renderer in stable state.
		Stopped,///< Renderer stopped.
		Error,///< Render has an error.
	};

	static auto getState() -> State { return m_internalState; }

private:
	/// The state of the renderer.
	static State m_internalState;
};

}// namespace owl::sound
