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

CameraOrtho::CameraOrtho(const float iLeft, const float iRight, const float iBottom, const float iTop)
	: m_viewMatrix(1.0f) {
	OWL_PROFILE_FUNCTION()
	setProjection(iLeft, iRight, iBottom, iTop, -1.0f, 1.0f);
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void CameraOrtho::setProjection(const float iLeft, const float iRight, const float iBottom, const float iTop,
                                const float iNear, const float iFar) {
	OWL_PROFILE_FUNCTION()

	m_projectionMatrix = glm::ortho(iLeft, iRight, iBottom, iTop, iNear, iFar);
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void CameraOrtho::recalculateViewMatrix() {
	OWL_PROFILE_FUNCTION()
	glm::mat4 transform;
	if (RenderCommand::getAPI() == RenderAPI::Type::Vulkan) {
		transform = glm::translate(glm::mat4(1.0f), m_position) *
		            glm::rotate(glm::mat4(1.0f), glm::radians(-m_rotation), glm::vec3(0, 0, 1));
	} else {
		transform = glm::translate(glm::mat4(1.0f), m_position) *
		            glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));
	}

	m_viewMatrix = glm::inverse(transform);
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

}// namespace owl::renderer
