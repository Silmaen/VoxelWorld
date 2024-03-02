/**
 * @file AirSpeed.h
 * @author Silmaen
 * @date 18/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "BaseGauge.h"

namespace drone::panels::gauge {
/**
 * @brief Class AirSpeed
 */
class AirSpeed final : public BaseGauge {
public:
	/**
	 * @brief Constructor.
	 */
	AirSpeed();
	/**
	 * @brief Destructor.
	 */
	~AirSpeed() override;
	/**
	 * @brief Copy constructor.
	 */
	AirSpeed(const AirSpeed&) = default;
	/**
	 * @brief Move constructor.
	 */
	AirSpeed(AirSpeed&&) = default;
	/**
	 * @brief Copy affectation operator.
	 */
	AirSpeed& operator=(const AirSpeed&) = default;
	/**
	 * @brief Move affectation operator.
	 */
	AirSpeed& operator=(AirSpeed&&) = default;

	/**
	 * @brief Draw the  gauge back ground.
	 */
	void drawBack() override;

	/**
	 * @brief Draw the gauge cursors.
	 */
	void drawCursors() override;

	/**
	 * @brief Define the velocity.
	 * @param vel The new velocity from UAV.
	 */
	void setVelocity(float vel) { velocity = vel; }

private:
	float velocity = 0.f;

	[[nodiscard]] float velocityToAngle() const;

	owl::shared<owl::renderer::Texture> background = nullptr;
	owl::shared<owl::renderer::Texture> cursor = nullptr;
};
} // namespace drone::panels::gauge
