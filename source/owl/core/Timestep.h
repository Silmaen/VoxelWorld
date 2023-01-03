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

namespace owl::core {

/**
 * @brief Class managing the Time Steps
 */
class OWL_API Timestep {
public:
	using clock = std::chrono::steady_clock;
	using time_point = clock::time_point;
	using duration = clock::duration;
	Timestep() : lastCall{clock::now()} { update(); }

	void update() {
		time_point tp = clock::now();
		delta = tp - lastCall;
		lastCall = tp;
	}

	[[nodiscard]] float getSeconds() const { return static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(delta).count()) / 1000.0f; }
	[[nodiscard]] float getMilliseconds() const { return static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(delta).count()) / 1000.0f; }
	[[nodiscard]] float getFps() const { return 1e3f / getMilliseconds(); }

private:
	time_point lastCall;
	duration delta;
};

}// namespace owl::core
