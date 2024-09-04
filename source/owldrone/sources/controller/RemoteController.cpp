/**
 * @file RemoteController.cpp
 * @author Silmaen
 * @date 21/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "RemoteController.h"

namespace drone::controller {

RemoteController::RemoteController(size_t motorNumber) { m_motors.resize(motorNumber); }

void RemoteController::setMotorRates(const std::vector<float>& mot) {
	if (mot.size() == m_motors.size()) {
		m_motors = mot;
	}
}

RemoteController::~RemoteController() = default;

}// namespace drone::controller
