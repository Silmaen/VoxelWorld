/**
 * @file Altitude.h
 * @author Silmaen
 * @date 23/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "BaseGauge.h"

namespace drone::panels::gauge {
/**
 * @brief Class Altitude
 */
class Altitude final : public BaseGauge {
public:
	/**
	 * @brief Constructor.
	 */
	Altitude();

	/**
	 * @brief Destructor.
	 */
	~Altitude() override;

	/**
	 * @brief Copy constructor.
	 */
	Altitude(const Altitude&) = default;

	/**
	 * @brief Move constructor.
	 */
	Altitude(Altitude&&) = default;

	/**
	 * @brief Copy affectation operator.
	 */
	Altitude& operator=(const Altitude&) = default;

	/**
	 * @brief Move affectation operator.
	 */
	Altitude& operator=(Altitude&&) = default;

	/**
	 * @brief Draw the  gauge back ground.
	 */
	void drawBack() override;

	/**
	 * @brief Draw the gauge cursors.
	 */
	void drawCursors() override;

	/**
	 * @brief Define the altitude.
	 * @param alt The new altitude from UAV.
	 */
	void setAltitude(float alt) { altitude = alt; }

private:
	float altitude = 0.f;

	[[nodiscard]] float altitudeToAngle(float divider = 1.0) const;

	owl::shared<owl::renderer::Texture> background = nullptr;
	owl::shared<owl::renderer::Texture> cursor = nullptr;
	owl::shared<owl::renderer::Texture> cursor10 = nullptr;
	owl::shared<owl::renderer::Texture> cursor100 = nullptr;
};
} // namespace drone::panels::gauge
