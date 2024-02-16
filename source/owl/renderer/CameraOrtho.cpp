/**
 * @file CameraOrtho.cpp
 * @author Silmaen
 * @date 10/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "CameraOrtho.h"
#include "RenderCommand.h"

namespace owl::renderer {

CameraOrtho::CameraOrtho(float left, float right, float bottom, float top)
	: viewMatrix(1.0f) {
	OWL_PROFILE_FUNCTION()
	setProjection(left, right, bottom, top, -1.0f, 1.0f);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}
void CameraOrtho::setProjection(float left, float right, float bottom, float top, float near, float far) {
	OWL_PROFILE_FUNCTION()

	projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void CameraOrtho::recalculateViewMatrix() {
	OWL_PROFILE_FUNCTION()
	glm::mat4 transform;
	if (RenderCommand::getAPI() == RenderAPI::Type::Vulkan) {
		transform = glm::translate(glm::mat4(1.0f), m_Position) *
					glm::rotate(glm::mat4(1.0f), glm::radians(-m_Rotation), glm::vec3(0, 0, 1));
	} else {
		transform = glm::translate(glm::mat4(1.0f), m_Position) *
					glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
	}

	viewMatrix = glm::inverse(transform);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

}// namespace owl::renderer
