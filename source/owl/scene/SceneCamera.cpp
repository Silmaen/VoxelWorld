/**
 * @file SceneCamera.cpp
 * @author Silmaen
 * @date 23/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace owl::scene {


SceneCamera::SceneCamera() { recalculateProjection(); }

void SceneCamera::setOrthographic(const float iSize, const float iNearClip, const float iFarClip) {
	m_projectionType = ProjectionType::Orthographic;
	m_orthographicSize = iSize;
	m_orthographicNear = iNearClip;
	m_orthographicFar = iFarClip;
	recalculateProjection();
}

void SceneCamera::setPerspective(const float iVerticalFov, const float iNearClip, const float iFarClip) {
	m_projectionType = ProjectionType::Perspective;
	m_perspectiveFOV = iVerticalFov;
	m_perspectiveNear = iNearClip;
	m_perspectiveFar = iFarClip;
	recalculateProjection();
}

void SceneCamera::setViewportSize(const uint32_t iWidth, const uint32_t iHeight) {
	OWL_CORE_ASSERT(iWidth > 0 && iHeight > 0, "Null viewport size")
	m_aspectRatio = static_cast<float>(iWidth) / static_cast<float>(iHeight);
	recalculateProjection();
}


void SceneCamera::recalculateProjection() {
	if (m_projectionType == ProjectionType::Perspective) {
		m_projection = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
	} else {
		const float orthoLeft = -m_orthographicSize * m_aspectRatio * 0.5f;
		const float orthoRight = m_orthographicSize * m_aspectRatio * 0.5f;
		const float orthoBottom = -m_orthographicSize * 0.5f;
		const float orthoTop = m_orthographicSize * 0.5f;
		m_projection = glm::ortho(orthoLeft, orthoRight,
								  orthoBottom, orthoTop, m_orthographicNear, m_orthographicFar);
	}
}

SceneCamera::~SceneCamera() = default;


}// namespace owl::scene
