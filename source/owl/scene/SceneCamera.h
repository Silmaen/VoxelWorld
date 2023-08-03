/**
 * @file SceneCamera.h
 * @author Silmaen
 * @date 23/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"
#include "renderer/Camera.h"

namespace owl::scene {

/**
 * @brief Class SceneCamera.
 */
class OWL_API SceneCamera : public renderer::Camera {
public:
	/**
	 * @brief Default constructor.
	 */
	SceneCamera();
	SceneCamera(const SceneCamera &) = default;
	SceneCamera(SceneCamera &&) = default;
	SceneCamera &operator=(const SceneCamera &) = default;
	SceneCamera &operator=(SceneCamera &&) = default;

	/**
	 * @brief Destructor.
	 */
	~SceneCamera() override;

	/**
	 * @brief Defines otho-graphic limits.
	 * @param size Size of the view.
	 * @param nearClip Near clip distance.
	 * @param farClip Far clip distance.
	 */
	void setOrthographic(float size, float nearClip, float farClip);

	/**
	 * @brief Defines perspective parameters.
	 * @param verticalFOV vertical Field of view.
	 * @param nearClip Near distance clipping.
	 * @param farClip Far distance clipping.
	 */
	void setPerspective(float verticalFOV, float nearClip, float farClip);

	/**
	 * @brief Defines the viewport.
	 * @param width Viewport's width.
	 * @param height Viewport's height.
	 */
	void setViewportSize(uint32_t width, uint32_t height);

	/**
	 * @brief Get actual orthographic size.
	 * @return The orthographic size.
	 */
	[[nodiscard]] float getOrthographicSize() const { return orthographicSize; }

	/**
	 * @brief Define the orthographic size.
	 * @param size The new size.
	 */
	void setOrthographicSize(float size) {
		orthographicSize = size;
		recalculateProjection();
	}

	/**
	 * @brief Projection types.
	 */
	enum struct ProjectionType {
		Perspective = 0,/// Perspective projection.
		Orthographic = 1/// Orthographic projection.
	};

	/**
	 * @brief Get the projection's type.
	 * @return The projection's type.
	 */
	[[nodiscard]] ProjectionType getProjectionType() const { return projectionType; }

	/**
	 * @brief Defines the projection's type.
	 * @param type New projection's type.
	 */
	void setProjectionType(ProjectionType type) {
		projectionType = type;
		recalculateProjection();
	}

	/**
	 * @brief Get the orthographic near clip distance.
	 * @return The orthographic near clip distance.
	 */
	[[nodiscard]] float getOrthographicNearClip() const { return orthographicNear; }

	/**
	 * @brief Defines orthographic near clip distance.
	 * @param nearClip New distance for near clipping.
	 */
	void setOrthographicNearClip(float nearClip) {
		orthographicNear = nearClip;
		recalculateProjection();
	}

	/**
	 * @brief Get the orthographic far clip distance.
	 * @return The orthographic far clip distance.
	 */
	[[nodiscard]] float getOrthographicFarClip() const { return orthographicFar; }

	/**
	 * @brief Defines orthographic far clip distance.
	 * @param farClip New distance for far clipping.
	 */
	void setOrthographicFarClip(float farClip) {
		orthographicFar = farClip;
		recalculateProjection();
	}

	/**
	 * @brief Get the perspective vertical field of vew.
	 * @return The perspective vertical field of view.
	 */
	[[nodiscard]] float getPerspectiveVerticalFOV() const { return perspectiveFOV; }

	/**
	 * @brief Defines perspective vertical field of view.
	 * @param verticalFov New vertical field of view.
	 */
	void setPerspectiveVerticalFOV(float verticalFov) {
		perspectiveFOV = verticalFov;
		recalculateProjection();
	}

	/**
	 * @brief Get the perspective near clip distance.
	 * @return The perspective near clip distance.
	 */
	[[nodiscard]] float getPerspectiveNearClip() const { return perspectiveNear; }

	/**
	 * @brief Defines perspective near clip distance.
	 * @param nearClip New distance for near clipping.
	 */
	void setPerspectiveNearClip(float nearClip) {
		perspectiveNear = nearClip;
		recalculateProjection();
	}

	/**
	 * @brief Get the perspective far clip distance.
	 * @return The perspective far clip distance.
	 */
	[[nodiscard]] float getPerspectiveFarClip() const { return perspectiveFar; }

	/**
	 * @brief Defines perspective far clip distance.
	 * @param farClip New distance for far clipping.
	 */
	void setPerspectiveFarClip(float farClip) {
		perspectiveFar = farClip;
		recalculateProjection();
	}

private:
	/**
	 * @brief Recompute projections.
	 */
	void recalculateProjection();
	/// Actual projection's type.
	ProjectionType projectionType = ProjectionType::Orthographic;

	/// Perspective field of view.
	float perspectiveFOV = glm::radians(45.0f);
	/// Perspective near clip distance.
	float perspectiveNear = 0.01f;
	/// Perspective far clip distance.
	float perspectiveFar = 1000.0f;
	/// Orthographic size.
	float orthographicSize = 10.0f;
	/// Orthographic near clip distance.
	float orthographicNear = -1.0f;
	/// Orthographic far clip distance.
	float orthographicFar = 1.0f;
	/// Aspect ratio.
	float aspectRatio = 0.0f;
};

}// namespace owl::scene
