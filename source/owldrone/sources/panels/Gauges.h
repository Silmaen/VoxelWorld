/**
 * @file Gauges.h
 * @author Silmaen
 * @date 18/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "BasePanel.h"
#include "gauge/BaseGauge.h"

namespace drone::panels {

/**
 * @brief Class Gauges
 */
class Gauges final: public BasePanel {
public:
	/**
	 * @brief Constructor.
	 */
	Gauges();

	/**
	 * @brief Destructor.
	 */
	~Gauges() override;

	/**
	 * @brief Copy constructor.
	 */
	Gauges(const Gauges &) = default;

	/**
	 * @brief Move constructor.
	 */
	Gauges(Gauges &&) = default;

	/**
	 * @brief Copy affectation operator.
	 */
	Gauges &operator=(const Gauges &) = default;

	/**
	 * @brief Move affectation operator.
	 */
	Gauges &operator=(Gauges &&) = default;

	/**
	 * @brief Update panel Status.
	 * @param ts The Time delta of the frame.
	 */
	void onUpdate(const owl::core::Timestep &ts) override;

	/**
	 * @brief Do the rendering.
	 */
	void onRender() override;

private:
	/// the frame buffer for this panel.
	owl::shared<owl::renderer::Framebuffer> framebuffer;
	/// Size of the viewport
	glm::vec2 viewportSize = {0.0f, 0.0f};
	/// View port bounds
	glm::vec2 viewportBounds[2] = {{0.0f, 0.0f}, {0.0f, 0.0f}};

	bool viewportFocused = false;
	bool viewportHovered = false;

	// The gauges..
	std::vector<owl::shared<gauge::BaseGauge>> gauges;

	/// The camera
	owl::shared<owl::renderer::CameraOrtho> camera;
};

}// namespace drone::panels
