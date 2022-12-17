/**
 * @file baseLayer.h
 * @author Silmaen
 * @date 10/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "owl.h"

/**
 * @brief Class baseLayer
 */
class baseLayer : public owl::core::layer::Layer {
public:
	baseLayer();

	void onUpdate(const owl::core::Timestep &ts) override;
	void onEvent(owl::event::Event &event) override;
	void onImGuiRender(const owl::core::Timestep &ts) override;

private:
	owl::shrd<owl::renderer::VertexArray> square1;

	owl::shrd<owl::renderer::VertexArray> squareVA;
	owl::shrd<owl::renderer::Texture2D> texture;
	owl::shrd<owl::renderer::Texture2D> textureLogo;

	owl::input::CameraOrthoController cameraController;
};