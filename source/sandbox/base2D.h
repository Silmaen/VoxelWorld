/**
 * @file base2D.h
 * @author Silmaen
 * @date 18/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "owl.h"

/**
 * @brief Class base2D
 */
class base2D: public owl::core::layer::Layer {
public:
	/**
	 * @brief Default constructor.
	 */
	base2D();
	/**
	 * @brief Destructor.
	 */
	virtual ~base2D() = default;

	void onAttach() override;
	void onDetach() override;
	void onUpdate(const owl::core::Timestep &ts) override;
	void onEvent(owl::event::Event &event) override;
	void onImGuiRender(const owl::core::Timestep &ts) override;

private:
	owl::input::CameraOrthoController cameraController;

	owl::shrd<owl::renderer::VertexArray> squareVA;
	owl::shrd<owl::renderer::Texture> checkerboardTexture;
	glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	owl::shrd<owl::renderer::Framebuffer> framebuffer;
};
