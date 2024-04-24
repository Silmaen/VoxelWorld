/**
 * @file CameraOrthoController.cpp
 * @author Silmaen
 * @date 17/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "CameraOrthoController.h"
#include "Input.h"

namespace owl::input {

CameraOrthoController::CameraOrthoController(const float iAspectRatio, const bool iRotation) :
	m_aspectRatio{iAspectRatio},
	m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel),
	m_rotation{iRotation} {}

void CameraOrthoController::onUpdate(const core::Timestep &iTimeStep) {
	OWL_PROFILE_FUNCTION()

	if (Input::isKeyPressed(input::key::A)) {
		m_cameraPosition.x -= static_cast<float>(cos(static_cast<double>(glm::radians(m_cameraRotation)))) *
				m_cameraTranslationSpeed * iTimeStep.getSeconds();
		m_cameraPosition.y -= static_cast<float>(sin(static_cast<double>(glm::radians(m_cameraRotation)))) *
				m_cameraTranslationSpeed * iTimeStep.getSeconds();
	} else if (Input::isKeyPressed(input::key::D)) {
		m_cameraPosition.x += static_cast<float>(cos(static_cast<double>(glm::radians(m_cameraRotation)))) *
				m_cameraTranslationSpeed * iTimeStep.getSeconds();
		m_cameraPosition.y += static_cast<float>(sin(static_cast<double>(glm::radians(m_cameraRotation)))) *
				m_cameraTranslationSpeed * iTimeStep.getSeconds();
	}
	if (Input::isKeyPressed(input::key::W)) {
		m_cameraPosition.x += static_cast<float>(sin(static_cast<double>(glm::radians(m_cameraRotation)))) *
				m_cameraTranslationSpeed * iTimeStep.getSeconds();
		m_cameraPosition.y += static_cast<float>(cos(static_cast<double>(glm::radians(m_cameraRotation)))) *
				m_cameraTranslationSpeed * iTimeStep.getSeconds();
	} else if (Input::isKeyPressed(input::key::S)) {
		m_cameraPosition.x -= static_cast<float>(sin(static_cast<double>(glm::radians(m_cameraRotation)))) *
				m_cameraTranslationSpeed * iTimeStep.getSeconds();
		m_cameraPosition.y -= static_cast<float>(cos(static_cast<double>(glm::radians(m_cameraRotation)))) *
				m_cameraTranslationSpeed * iTimeStep.getSeconds();
	}
	if (m_rotation) {
		if (Input::isKeyPressed(input::key::Q))
			m_cameraRotation += m_cameraRotationSpeed * iTimeStep.getSeconds();
		if (Input::isKeyPressed(input::key::E))
			m_cameraRotation -= m_cameraRotationSpeed * iTimeStep.getSeconds();
		if (m_cameraRotation > 180.0f)
			m_cameraRotation -= 360.0f;
		else if (m_cameraRotation <= -180.0f)
			m_cameraRotation += 360.0f;
		m_camera.setRotation(m_cameraRotation);
	}
	m_camera.setPosition(m_cameraPosition);
	m_cameraTranslationSpeed = m_zoomLevel;
}

void CameraOrthoController::onEvent(event::Event &ioEvent) {
	OWL_PROFILE_FUNCTION()

	event::EventDispatcher dispatcher(ioEvent);
	dispatcher.dispatch<event::MouseScrolledEvent>([this](auto &&PH1) {
		return onMouseScrolled(std::forward<decltype(PH1)>(PH1));
	});
	dispatcher.dispatch<event::WindowResizeEvent>([this](auto &&PH1) {
		return onWindowResized(std::forward<decltype(PH1)>(PH1));
	});
}

bool CameraOrthoController::onMouseScrolled(const event::MouseScrolledEvent &iEvent) {
	OWL_PROFILE_FUNCTION()

	m_zoomLevel -= iEvent.getYOff() * 0.25f;
	m_zoomLevel = std::max(m_zoomLevel, 0.25f);
	m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
	return false;
}

bool CameraOrthoController::onWindowResized(const event::WindowResizeEvent &iEvent) {
	OWL_PROFILE_FUNCTION()

	onResize(iEvent.getSize());
	return false;
}

void CameraOrthoController::onResize(const math::FrameSize &iSize) {
	m_aspectRatio = iSize.ratio();
	m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
}

}// namespace owl::input
