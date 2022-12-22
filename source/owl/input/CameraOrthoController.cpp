/**
 * @file CameraOrthoController.cpp
 * @author Silmaen
 * @date 17/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "CameraOrthoController.h"
#include "Input.h"

namespace owl::input {

CameraOrthoController::CameraOrthoController(float aspectRatio_, bool rotation_) : aspectRatio{aspectRatio_},
																				   camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel),
																				   rotation{rotation_} {
}

void CameraOrthoController::onUpdate(core::Timestep ts) {
	OWL_PROFILE_FUNCTION()

	if (Input::isKeyPressed(input::key::A)) {
		cameraPosition.x -= static_cast<float>(cos(static_cast<double>(glm::radians(cameraRotation)))) * cameraTranslationSpeed * ts.getSeconds();
		cameraPosition.y -= static_cast<float>(sin(static_cast<double>(glm::radians(cameraRotation)))) * cameraTranslationSpeed * ts.getSeconds();
	} else if (Input::isKeyPressed(input::key::D)) {
		cameraPosition.x += static_cast<float>(cos(static_cast<double>(glm::radians(cameraRotation)))) * cameraTranslationSpeed * ts.getSeconds();
		cameraPosition.y += static_cast<float>(sin(static_cast<double>(glm::radians(cameraRotation)))) * cameraTranslationSpeed * ts.getSeconds();
	}
	if (Input::isKeyPressed(input::key::W)) {
		cameraPosition.x += static_cast<float>(sin(static_cast<double>(glm::radians(cameraRotation)))) * cameraTranslationSpeed * ts.getSeconds();
		cameraPosition.y += static_cast<float>(cos(static_cast<double>(glm::radians(cameraRotation)))) * cameraTranslationSpeed * ts.getSeconds();
	} else if (Input::isKeyPressed(input::key::S)) {
		cameraPosition.x -= static_cast<float>(sin(static_cast<double>(glm::radians(cameraRotation)))) * cameraTranslationSpeed * ts.getSeconds();
		cameraPosition.y -= static_cast<float>(cos(static_cast<double>(glm::radians(cameraRotation)))) * cameraTranslationSpeed * ts.getSeconds();
	}
	if (rotation) {
		if (Input::isKeyPressed(input::key::Q))
			cameraRotation += cameraRotationSpeed * ts.getSeconds();
		if (Input::isKeyPressed(input::key::E))
			cameraRotation -= cameraRotationSpeed * ts.getSeconds();
		if (cameraRotation > 180.0f)
			cameraRotation -= 360.0f;
		else if (cameraRotation <= -180.0f)
			cameraRotation += 360.0f;
		camera.setRotation(cameraRotation);
	}
	camera.setPosition(cameraPosition);
	cameraTranslationSpeed = zoomLevel;
}

void CameraOrthoController::onEvent(event::Event &e) {
	OWL_PROFILE_FUNCTION()

	event::EventDispatcher dispatcher(e);
	dispatcher.dispatch<event::MouseScrolledEvent>([this](auto &&PH1) { return onMouseScrolled(std::forward<decltype(PH1)>(PH1)); });
	dispatcher.dispatch<event::WindowResizeEvent>([this](auto &&PH1) { return onWindowResized(std::forward<decltype(PH1)>(PH1)); });
}

bool CameraOrthoController::onMouseScrolled(event::MouseScrolledEvent &e) {
	OWL_PROFILE_FUNCTION()

	zoomLevel -= e.getYOff() * 0.25f;
	zoomLevel = std::max(zoomLevel, 0.25f);
	camera.setProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
	return false;
}

bool CameraOrthoController::onWindowResized(event::WindowResizeEvent &e) {
	OWL_PROFILE_FUNCTION()

	onResize(static_cast<float>(e.getWidth()), static_cast<float>(e.getHeight()));
	return false;
}

void CameraOrthoController::onResize(float width, float height){
	aspectRatio = width / height;
	camera.setProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
}

}// namespace owl::input
