/**
 * @file Timestep.h
 * @author Silmaen
 * @date 10/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

namespace owl::core {
constexpr float sMillis{1000.f};
/**
 * @brief Time Steps.
 */
class OWL_API Timestep {
public:
	/// The type of clock.
	using clock = std::chrono::steady_clock;
	/// The type of time point.
	using timePoint = clock::time_point;
	/// The tipe of duration.
	using duration = clock::duration;

	/**
	 * @brief Default constructor.
	 */
	Timestep() : m_lastCall{clock::now()} {
		m_statFps.resize(maxIndex, 0.0);
		update();
	}

	/**
	 * @brief Time step update.
	 */
	void update() {
		const timePoint tp = clock::now();
		m_delta = tp - m_lastCall;
		m_lastCall = tp;
		m_statFps[m_index] = getFps();
		m_index = (m_index + 1) % maxIndex;
		++m_frameId;
	}

	/**
	 * @brief Force the step with a given duration
	 * @param iDelta The duration.
	 */
	void forceUpdate(const duration iDelta) {
		m_delta = iDelta;
		m_lastCall += iDelta;
		m_statFps[m_index] = getFps();
		m_index = (m_index + 1) % maxIndex;
		++m_frameId;
	}

	/**
	 * @brief Get the seconds elapsed since last update.
	 * @return Seconds elapsed.
	 */
	[[nodiscard]] float getSeconds() const {
		return static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(m_delta).count()) / sMillis;
	}

	/**
	 * @brief Get the milliseconds elapsed since last update.
	 * @return Milliseconds elapsed.
	 */
	[[nodiscard]] float getMilliseconds() const {
		return static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(m_delta).count()) / sMillis;
	}

	/**
	 * @brief Get the mean number of update call in one second.
	 * @return The Frame per second number.
	 */
	[[nodiscard]] float getFps() const { return sMillis / getMilliseconds(); }

	/**
	 * @brief Get the mean number of update call in one second.
	 * @return The Frame per second number.
	 */
	[[nodiscard]] float getStabilizedFps() const;

	/**
	 * @brief Get the frame number.
	 * @return The Frame number.
	 */
	[[nodiscard]] uint64_t getFrameNumber() const { return m_frameId; }

private:
	/// Last update call point.
	timePoint m_lastCall;
	/// The delta with the previous update call.
	duration m_delta{};
	/// Number of the current frame.
	uint64_t m_frameId{0};
	/// Stabilized fps counters.
	std::vector<float> m_statFps;
	/// index in the stats.
	size_t m_index{0};
	/// Max va in stats.
	static constexpr size_t maxIndex{20};
};
}// namespace owl::core
