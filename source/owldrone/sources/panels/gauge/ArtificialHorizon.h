/**
 * @file ArtificialHorizon.h
 * @author Silmaen
 * @date 24/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once
#include "BaseGauge.h"

namespace drone::panels::gauge {

/**
 * @brief Class ArtificialHorizon
 */
class ArtificialHorizon final: public BaseGauge {
public:
	/**
	 * @brief Constructor.
	 */
	ArtificialHorizon();

	/**
	 * @brief Destructor.
	 */
	~ArtificialHorizon() override;

	/**
	 * @brief Copy constructor.
	 */
	ArtificialHorizon(const ArtificialHorizon &) = default;

	/**
	 * @brief Move constructor.
	 */
	ArtificialHorizon(ArtificialHorizon &&) = default;

	/**
	 * @brief Copy affectation operator.
	 */
	ArtificialHorizon &operator=(const ArtificialHorizon &) = default;

	/**
	 * @brief Move affectation operator.
	 */
	ArtificialHorizon &operator=(ArtificialHorizon &&) = default;

	/**
	 * @brief Draw the  gauge back ground.
	 */
	void drawBack() override;

	/**
	 * @brief Draw the gauge cursors.
	 */
	void drawCursors() override;

	/**
	 * @brief Defines th pitch and roll of the gauge
	 * @param p Pitch.
	 * @param r Roll.
	 */
	void setPitchRoll(float p, float r) {
		pitch = p;
		roll = r;
	}

private:
	float pitch = 0;
	float roll = 0;

	owl::shared<owl::renderer::Texture> background = nullptr;
	owl::shared<owl::renderer::Texture> rollRing = nullptr;
	owl::shared<owl::renderer::Texture> olivePitch = nullptr;
	owl::shared<owl::renderer::Texture> cursor = nullptr;
};

}// namespace drone::panels::gauge
