/**
 * @file Timestep.cpp
 * @author Silmaen
 * @date 16/02/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Timestep.h"

namespace owl::core {

float Timestep::getStabilizedFps() const {
	return statFps.empty() ? 0 : std::accumulate(statFps.begin(), statFps.end(), 0.f) / static_cast<float>(statFps.size());
}
}// namespace owl::core
