/**
 * @file SceneCamera.h
 * @author Silmaen
 * @date 23/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "renderer/Camera.h"

namespace owl::scene {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class SceneCamera
 */
class OWL_API SceneCamera : public renderer::Camera {
public:
	/**
	 * @brief Default copy constructor
	 */
	SceneCamera(const SceneCamera &) = default;
	/**
	 * @brief Default move constructor
	 */
	SceneCamera(SceneCamera &&) = default;
	/**
	 * @brief Default copy assignation
	 * @return this
	 */
	SceneCamera &operator=(const SceneCamera &) = default;
	/**
	 * @brief Default move assignation
	 * @return this
	 */
	SceneCamera &operator=(SceneCamera &&) = default;
	/**
	 * @brief Default constructor.
	 */
	SceneCamera();
	/**
	 * @brief Destructor.
	 */
	~SceneCamera() override = default;

	/**
	 * @brief Defines othographics limits
	 * @param size Size of the view
	 * @param nearClip Near clip distance
	 * @param farClip Far clip distance
	 */
	void setOrthographic(float size, float nearClip, float farClip);

	void setPerspective(float verticalFOV, float nearClip, float farClip);
	/**
	 * @brief Defines the viewport
	 * @param width Viewport's width
	 * @param height Viewport's height
	 */
	void setViewportSize(uint32_t width, uint32_t height);

	/**
	 * @brief Get actual orthographic size
	 * @return The orthographic size
	 */
	[[nodiscard]] float getOrthographicSize() const { return orthographicSize; }

	/**
	 * @brief Define the orthographic size
	 * @param size The new size
	 */
	void setOrthographicSize(float size) {
		orthographicSize = size;
		recalculateProjection();
	}

	enum struct ProjectionType {
		Perspective = 0,
		Orthographic = 1
	};

	[[nodiscard]] ProjectionType getProjectionType() const { return projectionType; }
	void setProjectionType(ProjectionType type) {
		projectionType = type;
		recalculateProjection();
	}
	[[nodiscard]] float getOrthographicNearClip() const { return orthographicNear; }
	void setOrthographicNearClip(float nearClip) {
		orthographicNear = nearClip;
		recalculateProjection();
	}
	[[nodiscard]] float getOrthographicFarClip() const { return orthographicFar; }
	void setOrthographicFarClip(float farClip) {
		orthographicFar = farClip;
		recalculateProjection();
	}

	float getPerspectiveVerticalFOV() const { return perspectiveFOV; }
	void setPerspectiveVerticalFOV(float verticalFov) {
		perspectiveFOV = verticalFov;
		recalculateProjection();
	}
	float getPerspectiveNearClip() const { return perspectiveNear; }
	void setPerspectiveNearClip(float nearClip) {
		perspectiveNear = nearClip;
		recalculateProjection();
	}
	float getPerspectiveFarClip() const { return perspectiveFar; }
	void setPerspectiveFarClip(float farClip) {
		perspectiveFar = farClip;
		recalculateProjection();
	}

private:
	void recalculateProjection();
	ProjectionType projectionType = ProjectionType::Orthographic;

	float perspectiveFOV = glm::radians(45.0f);
	float perspectiveNear = 0.01f;
	float perspectiveFar = 1000.0f;
	float orthographicSize = 10.0f;
	float orthographicNear = -1.0f;
	float orthographicFar = 1.0f;

	float aspectRatio = 0.0f;
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::scene
