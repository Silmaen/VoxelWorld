/**
 * @file CameraEditor.h
 * @author Silmaen
 * @date 30/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Camera.h"
#include "core/Timestep.h"
#include "event/Event.h"
#include "event/MouseEvent.h"

#include <glm/glm.hpp>

namespace owl::renderer {

/**
 * @brief Class CameraEditor.
 */
class OWL_API CameraEditor : public Camera {
public:
	CameraEditor() = default;
	~CameraEditor() override = default;
	CameraEditor(const CameraEditor &) = default;
	CameraEditor(CameraEditor &&) = default;
	CameraEditor &operator=(const CameraEditor &) = default;
	CameraEditor &operator=(CameraEditor &&) = default;

	/**
	 * @brief Constructor
	 * @param fov Field of view.
	 * @param aspectRatio Aspect rtio.
	 * @param nearClip Near clip distance.
	 * @param farClip Far clip distance.
	 */
	CameraEditor(float fov, float aspectRatio, float nearClip, float farClip);

	/**
	 * @brief Update the camera (Need input setup).
	 * @param ts The timestamp.
	 */
	void onUpdate(core::Timestep ts);

	/**
	 * @brief Treat an event.
	 * @param event Teh event to treat.
	 */
	void onEvent(event::Event &event);

	/**
	 * @brief Get the camera distance to the focal point.
	 * @return Camera distance to the focal point.
	 */
	[[nodiscard]] float getDistance() const { return distance; }

	/**
	 * @brief Set the camera distance to the focal point.
	 * @param distance_ The new distance to the focal point.
	 */
	void setDistance(float distance_) { distance = distance_; }

	/**
	 * @brief Set the camera viewport size.
	 * @param width New width.
	 * @param height New height.
	 */
	inline void setViewportSize(float width, float height) {
		viewportWidth = width;
		viewportHeight = height;
		updateProjection();
	}

	/**
	 * @brief Get the camera view matrix.
	 * @return Camera view matrix.
	 */
	[[nodiscard]] const glm::mat4 &getViewMatrix() const { return viewMatrix; }

	/**
	 * @brief Get the camera view projection matrix.
	 * @return Camera view projection matrix.
	 */
	[[nodiscard]] glm::mat4 getViewProjection() const { return projection * viewMatrix; }

	/**
	 * @brief Get the camera up vector.
	 * @return Camera up vector.
	 */
	[[nodiscard]] glm::vec3 getUpDirection() const;

	/**
	 * @brief Get the camera right vector.
	 * @return Camera right vector.
	 */
	[[nodiscard]] glm::vec3 getRightDirection() const;

	/**
	 * @brief Get the camera forward vector.
	 * @return Camera forward vector.
	 */
	[[nodiscard]] glm::vec3 getForwardDirection() const;
	/**
	 * @brief Get the camera position.
	 * @return Camera position.
	 */
	[[nodiscard]] const glm::vec3 &getPosition() const { return position; }

	/**
	 * @brief Get the camera orientation quaternion.
	 * @return Camera orientation.
	 */
	[[nodiscard]] glm::quat getOrientation() const;

	/**
	 * @brief Get the camera pitch.
	 * @return Camera pitch.
	 */
	[[nodiscard]] float getPitch() const { return pitch; }

	/**
	 * @brief Get the camera yaw.
	 * @return Camera yaw.
	 */
	[[nodiscard]] float getYaw() const { return yaw; }

private:
	/**
	 * @brief Update the projection matrix.
	 */
	void updateProjection();

	/**
	 * @brief Update the view matrix.
	 */
	void updateView();

	/**
	 * @brief Event when mouse is scrolled.
	 * @param e Mouse scroll event.
	 * @return If treated.
	 */
	bool onMouseScroll(event::MouseScrolledEvent &e);

	/**
	 * @brief Mouse panoramic move.
	 * @param delta The X Y deltas.
	 */
	void mousePan(const glm::vec2 &delta);

	/**
	 * @brief Mouse rotation.
	 * @param delta The X Y deltas.
	 */
	void mouseRotate(const glm::vec2 &delta);

	/**
	 * @brief Mouse zoom.
	 * @param delta The delta.
	 */
	void mouseZoom(float delta);

	/**
	 * @brief Calculate the camera position.
	 * @return Camera position.
	 */
	[[nodiscard]] glm::vec3 calculatePosition() const;

	/**
	 * @brief Get the pan speeds.
	 * @return Pan speeds.
	 */
	[[nodiscard]] std::pair<float, float> panSpeed() const;

	/**
	 * @brief Get the rotation speed.
	 * @return The rotation speed.
	 */
	[[nodiscard]] float rotationSpeed() const;

	/**
	 * @brief Get the zoom speed.
	 * @return The zoom speed.
	 */
	[[nodiscard]] float zoomSpeed() const;

	/// Field of View.
	float FOV = 45.0f;
	/// Aspect ratio.
	float aspectRatio = 1.778f;
	/// Near clipping distance.
	float nearClip = 0.1f;
	/// Far clipping distance.
	float farClip = 1000.0f;

	/// Internal view matrix.
	glm::mat4 viewMatrix{};
	/// Camera position.
	glm::vec3 position = {0.0f, 0.0f, 0.0f};
	/// Camera Focal point.
	glm::vec3 focalPoint = {0.0f, 0.0f, 0.0f};

	/// Save mouse position for drag.
	glm::vec2 initialMousePosition = {0.0f, 0.0f};

	/// Camera distance.
	float distance = 10.0f;
	/// Camera's pitch.
	float pitch = 0.0f;
	/// Camera's yaw.
	float yaw = 0.0f;

	/// Viewport width.
	float viewportWidth = 1280;
	/// Viewport height.
	float viewportHeight = 720;
};

}// namespace owl::renderer
