/**
 * @file base2D.h
 * @author Silmaen
 * @date 18/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <owl.h>

namespace owl {

/**
 * @brief Class base2D
 */
class base2D final : public core::layer::Layer {
public:
	/**
	 * @brief Default constructor.
	 */
	base2D();
	base2D(const base2D &) = delete;
	base2D(base2D &&) = delete;
	base2D &operator=(const base2D &) = delete;
	base2D &operator=(base2D &&) = delete;
	/**
	 * @brief Destructor.
	 */
	~base2D() override = default;

	void onAttach() override;
	void onDetach() override;
	void onUpdate(const core::Timestep &ts) override;
	void onEvent(event::Event &event) override;
	void onImGuiRender(const core::Timestep &ts) override;

private:
	input::CameraOrthoController cameraController;

	glm::vec2 viewportSize = {0.0f, 0.0f};

	shared<renderer::Texture> checkerboardTexture;
	glm::vec4 squareColor = {0.2f, 0.3f, 0.8f, 1.0f};
	int32_t m_hoveredEntity = -1;
};

}// namespace owl
