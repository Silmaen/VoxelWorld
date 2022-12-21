/**
 * @file CameraOrtho.cpp
 * @author Silmaen
 * @date 10/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "CameraOrtho.h"
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#pragma clang diagnostic ignored "-Wdocumentation-deprecated-sync"
#pragma clang diagnostic ignored "-Wdocumentation"
#endif
#include <glm/gtc/matrix_transform.hpp>
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

namespace owl::renderer {

CameraOrtho::CameraOrtho(float left, float right, float bottom, float top)
	: projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), viewMatrix(1.0f) {
	OWL_PROFILE_FUNCTION()

	viewProjectionMatrix = projectionMatrix * viewMatrix;
}
void CameraOrtho::setProjection(float left, float right, float bottom, float top) {
	OWL_PROFILE_FUNCTION()

	projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void CameraOrtho::recalculateViewMatrix() {
	OWL_PROFILE_FUNCTION()

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
						  glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
	viewMatrix = glm::inverse(transform);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

}// namespace owl::renderer
