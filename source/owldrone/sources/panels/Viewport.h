/**
 * @file Viewport.h
 * @author Silmaen
 * @date 18/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "BasePanel.h"

namespace drone::panels {

/**
 * @brief Class Viewport
 */
class Viewport : public BasePanel {
public:
	/**
	 * @brief Constructor.
	 */
	Viewport();

	/**
	 * @brief Destructor.
	 */
	~Viewport() override;

	/**
	 * @brief Copy constructor.
	 */
	Viewport(const Viewport &) = default;

	/**
	 * @brief Move constructor.
	 */
	Viewport(Viewport &&) = default;

	/**
	 * @brief Copy affectation operator.
	 */
	Viewport &operator=(const Viewport &) = default;

	/**
	 * @brief Move affectation operator.
	 */
	Viewport &operator=(Viewport &&) = default;

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
};

}// namespace drone::panels
