/**
 * @file MotorRate.h
 * @author Silmaen
 * @date 24/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once
#include "BaseGauge.h"

namespace drone::panels::gauge {

/**
 * @brief Class MotorRate
 */
class MotorRate final: public BaseGauge {
public:
	/**
	 * @brief Constructor.
	 */
	MotorRate();

	/**
	 * @brief Destructor.
	 */
	~MotorRate() override;

	/**
	 * @brief Copy constructor.
	 */
	MotorRate(const MotorRate &) = default;

	/**
	 * @brief Move constructor.
	 */
	MotorRate(MotorRate &&) = default;

	/**
	 * @brief Copy affectation operator.
	 */
	MotorRate &operator=(const MotorRate &) = default;

	/**
	 * @brief Move affectation operator.
	 */
	MotorRate &operator=(MotorRate &&) = default;

	/**
	 * @brief Draw the  gauge back ground.
	 */
	void drawBack() override;

	/**
	 * @brief Draw the gauge cursors.
	 */
	void drawCursors() override;

	/**
	 * @brief Defines the motor rates.
	 * @param r Motor rates.
	 */
	void setMotorRates(const std::vector<float> &r) { rates = r; }

private:
	std::vector<float> rates;

	[[nodiscard]] float rateToAngle(uint8_t idx) const;

	owl::shared<owl::renderer::Texture> background = nullptr;
	owl::shared<owl::renderer::Texture> cursor1 = nullptr;
	owl::shared<owl::renderer::Texture> cursor2 = nullptr;
	owl::shared<owl::renderer::Texture> cursor3 = nullptr;
	owl::shared<owl::renderer::Texture> cursor4 = nullptr;
};

}// namespace drone::panels::gauge
