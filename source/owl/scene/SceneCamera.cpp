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


SceneCamera::SceneCamera() {
	recalculateProjection();
}

void SceneCamera::setOrthographic(float size, float nearClip, float farClip) {
	projectionType = ProjectionType::Orthographic;
	orthographicSize = size;
	orthographicNear = nearClip;
	orthographicFar = farClip;
	recalculateProjection();
}

void SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip) {
	projectionType = ProjectionType::Perspective;
	perspectiveFOV = verticalFOV;
	perspectiveNear = nearClip;
	perspectiveFar = farClip;
	recalculateProjection();
}

void SceneCamera::setViewportSize(uint32_t width, uint32_t height) {
	OWL_CORE_ASSERT(width > 0 && height > 0, "Null viewport size")
	aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	recalculateProjection();
}


void SceneCamera::recalculateProjection() {
	if (projectionType == ProjectionType::Perspective) {
		projection = glm::perspective(perspectiveFOV, aspectRatio, perspectiveNear, perspectiveFar);
	} else {
		float orthoLeft = -orthographicSize * aspectRatio * 0.5f;
		float orthoRight = orthographicSize * aspectRatio * 0.5f;
		float orthoBottom = -orthographicSize * 0.5f;
		float orthoTop = orthographicSize * 0.5f;

		projection = glm::ortho(orthoLeft, orthoRight,
								orthoBottom, orthoTop, orthographicNear, orthographicFar);
	}
}

SceneCamera::~SceneCamera() = default;


}// namespace owl::scene
