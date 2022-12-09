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
class baseLayer : public owl::core::layer::Layer{
public:
	baseLayer();
	void onUpdate(const owl::core::Timestep& ts) override;
	void onEvent([[maybe_unused]] owl::event::Event &event) override {}
	void onImGuiRender(const owl::core::Timestep& ts) override;
private:
	owl::shrd<owl::renderer::Shader> shader;
	owl::shrd<owl::renderer::VertexArray> vertexArray;

	owl::shrd<owl::renderer::Shader> blueShader;
	owl::shrd<owl::renderer::VertexArray> squareVA;

	owl::renderer::CameraOrtho camera;
	glm::vec3 cameraPosition;
	float cameraMoveSpeed = 5.0f;

	float cameraRotation = 0.0f;
	float cameraRotationSpeed = 180.0f;
};
