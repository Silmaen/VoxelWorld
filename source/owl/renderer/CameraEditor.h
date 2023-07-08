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


	void onUpdate(core::Timestep ts);

	void onEvent(event::Event &event);

	[[nodiscard]] float getDistance() const { return distance; }
	void setDistance(float distance_) { distance = distance_; }

	inline void setViewportSize(float width, float height) {
		viewportWidth = width;
		viewportHeight = height;
		updateProjection();
	}

	[[nodiscard]] const glm::mat4 &getViewMatrix() const { return viewMatrix; }
	[[nodiscard]] glm::mat4 getViewProjection() const { return projection * viewMatrix; }
	[[nodiscard]] glm::vec3 getUpDirection() const;
	[[nodiscard]] glm::vec3 getRightDirection() const;
	[[nodiscard]] glm::vec3 getForwardDirection() const;
	[[nodiscard]] const glm::vec3 &getPosition() const { return position; }
	[[nodiscard]] glm::quat getOrientation() const;

	[[nodiscard]] float getPitch() const { return pitch; }
	[[nodiscard]] float getYaw() const { return yaw; }

private:
	void updateProjection();
	void updateView();

	bool onMouseScroll(event::MouseScrolledEvent &e);

	void mousePan(const glm::vec2 &delta);
	void mouseRotate(const glm::vec2 &delta);
	void mouseZoom(float delta);

	[[nodiscard]] glm::vec3 calculatePosition() const;

	[[nodiscard]] std::pair<float, float> panSpeed() const;
	[[nodiscard]] float rotationSpeed() const;
	[[nodiscard]] float zoomSpeed() const;

	float FOV = 45.0f;
	float aspectRatio = 1.778f;
	float nearClip = 0.1f;
	float farClip = 1000.0f;

	glm::mat4 viewMatrix{};
	glm::vec3 position = {0.0f, 0.0f, 0.0f};
	glm::vec3 focalPoint = {0.0f, 0.0f, 0.0f};

	glm::vec2 initialMousePosition = {0.0f, 0.0f};

	float distance = 10.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;

	float viewportWidth = 1280;
	float viewportHeight = 720;
};

}// namespace owl::renderer
