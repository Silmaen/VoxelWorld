/**
 * @file CameraEditor.cpp
 * @author Silmaen
 * @date 30/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "CameraEditor.h"

#include "input/Input.h"

#include <glm/gtx/quaternion.hpp>

namespace owl::renderer {

Camera::~Camera() = default;

CameraEditor::CameraEditor(float fov, float aspectRatio_, float nearClip_, float farClip_) : Camera(glm::perspective(glm::radians(fov), aspectRatio_, nearClip_, farClip_)),
																							 FOV(fov), aspectRatio(aspectRatio_), nearClip(nearClip_), farClip(farClip_) {
	updateView();
}

CameraEditor::~CameraEditor() = default;

void CameraEditor::onUpdate([[maybe_unused]] core::Timestep ts) {
	if (input::Input::isKeyPressed(input::key::LeftAlt)) {
		const glm::vec2 &mouse{input::Input::getMouseX(), input::Input::getMouseY()};
		glm::vec2 delta = (mouse - initialMousePosition) * 0.003f;
		initialMousePosition = mouse;
		if (input::Input::isMouseButtonPressed(input::mouse::ButtonMiddle))
			mousePan(delta);
		else if (input::Input::isMouseButtonPressed(input::mouse::ButtonLeft))
			mouseRotate(delta);
		else if (input::Input::isMouseButtonPressed(input::mouse::ButtonRight))
			mouseZoom(delta.y);
	}
	updateView();
}

void CameraEditor::onEvent(event::Event &event) {
	event::EventDispatcher dispatcher(event);
	dispatcher.dispatch<event::MouseScrolledEvent>(
			[&](auto &&TZ1) { return onMouseScroll(std::forward<decltype(TZ1)>(TZ1)); });
}

glm::vec3 CameraEditor::getUpDirection() const {
	return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 CameraEditor::getRightDirection() const {
	return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 CameraEditor::getForwardDirection() const {
	return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::quat CameraEditor::getOrientation() const {
	return {glm::vec3(-pitch, -yaw, 0.0f)};
}

void CameraEditor::updateProjection() {
	aspectRatio = viewportWidth / viewportHeight;
	projection = glm::perspective(glm::radians(FOV), aspectRatio, nearClip, farClip);
}

void CameraEditor::updateView() {
	// yaw = pitch = 0.0f; // Lock the camera's rotation
	position = calculatePosition();

	glm::quat orientation = getOrientation();
	viewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
	viewMatrix = glm::inverse(viewMatrix);
}

bool CameraEditor::onMouseScroll(event::MouseScrolledEvent &e) {
	float delta = e.getYOff() * 0.1f;
	mouseZoom(delta);
	updateView();
	return false;
}

void CameraEditor::mousePan(const glm::vec2 &delta) {
	auto [xSpeed, ySpeed] = panSpeed();
	focalPoint += -getRightDirection() * delta.x * xSpeed * distance;
	focalPoint += getUpDirection() * delta.y * ySpeed * distance;
}

void CameraEditor::mouseRotate(const glm::vec2 &delta) {
	float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
	yaw += yawSign * delta.x * rotationSpeed();
	pitch += delta.y * rotationSpeed();
}

void CameraEditor::mouseZoom(float delta) {
	distance -= delta * zoomSpeed();
	if (distance < 1.0f) {
		focalPoint += getForwardDirection();
		distance = 1.0f;
	}
}

glm::vec3 CameraEditor::calculatePosition() const {
	return focalPoint - getForwardDirection() * distance;
}

std::pair<float, float> CameraEditor::panSpeed() const {
	float x = std::min(viewportWidth / 1000.0f, 2.4f);// max = 2.4f
	float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

	float y = std::min(viewportHeight / 1000.0f, 2.4f);// max = 2.4f
	float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

	return {xFactor, yFactor};
}

float CameraEditor::rotationSpeed() const {
	return 0.8f;
}

float CameraEditor::zoomSpeed() const {
	float distance_ = distance * 0.2f;
	distance_ = std::max(distance_, 0.0f);
	float speed = distance_ * distance_;
	speed = std::min(speed, 100.0f);// max speed = 100
	return speed;
}

}// namespace owl::renderer
