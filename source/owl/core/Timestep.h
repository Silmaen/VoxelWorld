/**
 * @file Timestep.h
 * @author Silmaen
 * @date 10/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"
#include <chrono>
#include <numeric>
#include <queue>

namespace owl::core {

/**
 * @brief Class managing the Time Steps.
 */
class OWL_API Timestep {
public:
	/// The type of clock.
	using clock = std::chrono::steady_clock;
	/// The type of time point.
	using time_point = clock::time_point;
	/// The tipe of duration.
	using duration = clock::duration;

	/**
	 * @brief Default constructor.
	 */
	Timestep() : lastCall{clock::now()} {
		statFps.resize(maxIndex, 0.0);
		update();
	}

	/**
	 * @brief Time step update.
	 */
	void update() {
		time_point tp = clock::now();
		delta = tp - lastCall;
		lastCall = tp;
		if (delta.count() > 0) {
			statFps[index] = getFps();
			index = (index + 1) % maxIndex;
		}
	}

	/**
	 * @brief Get the seconds elapsed since last update.
	 * @return Seconds elapsed.
	 */
	[[nodiscard]] float getSeconds() const { return static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(delta).count()) / 1000.0f; }

	/**
	 * @brief Get the milliseconds elapsed since last update.
	 * @return Milliseconds elapsed.
	 */
	[[nodiscard]] float getMilliseconds() const { return static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(delta).count()) / 1000.0f; }

	/**
	 * @brief Get the mean number of update call in one second.
	 * @return The Frame per second number.
	 */
	[[nodiscard]] float getFps() const { return 1e3f / getMilliseconds(); }

	/**
	 * @brief Get the mean number of update call in one second.
	 * @return The Frame per second number.
	 */
	[[nodiscard]] float getStabilizedFps() const {
		return statFps.empty() ? 0 : std::accumulate(statFps.begin(), statFps.end(), 0.f) / static_cast<float>(statFps.size());
	}

private:
	/// Last update call point.
	time_point lastCall{};
	/// The delta with the previous update call.
	duration delta{};
	/// Stabilized fps counters.
	std::vector<float> statFps;
	/// index in the stats.
	size_t index = 0;
	/// Max va in stats.
	static constexpr size_t maxIndex = 20;
};

}// namespace owl::core
