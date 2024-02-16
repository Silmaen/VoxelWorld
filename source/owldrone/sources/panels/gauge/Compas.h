/**
 * @file Compas.h
 * @author Silmaen
 * @date 24/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once
#include "BaseGauge.h"

namespace drone::panels::gauge {

/**
 * @brief Class Compas
 */
class Compas final: public BaseGauge {
public:
	/**
	 * @brief Constructor.
	 */
	Compas();

	/**
	 * @brief Destructor.
	 */
	~Compas() override;

	/**
	 * @brief Copy constructor.
	 */
	Compas(const Compas &) = default;

	/**
	 * @brief Move constructor.
	 */
	Compas(Compas &&) = default;

	/**
	 * @brief Copy affectation operator.
	 */
	Compas &operator=(const Compas &) = default;

	/**
	 * @brief Move affectation operator.
	 */
	Compas &operator=(Compas &&) = default;

	/**
	 * @brief Draw the  gauge back ground.
	 */
	void drawBack() override;

	/**
	 * @brief Draw the gauge cursors.
	 */
	void drawCursors() override;

	/**
	 * @brief Defines the heading.
	 * @param h The heading.
	 */
	void setHeading(float h) { heading = h; }

private:
	float heading = 0;
	float headingToAngle();

	owl::shared<owl::renderer::Texture> background = nullptr;
	owl::shared<owl::renderer::Texture> cursor = nullptr;
};

}// namespace drone::panels::gauge
