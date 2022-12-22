/**
 * @file CameraOrthoController.h
 * @author Silmaen
 * @date 17/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Timestep.h"
#include "event/AppEvent.h"
#include "event/MouseEvent.h"
#include "renderer/CameraOrtho.h"
#include <glm/vec3.hpp>

namespace owl::input {

/**
 * @brief Class CameraOrthoController
 */
class OWL_API CameraOrthoController {
public:
	/**
	 * @brief Default copy constructor
	 */
	CameraOrthoController(const CameraOrthoController &) = default;
	/**
	 * @brief Default move constructor
	 */
	CameraOrthoController(CameraOrthoController &&) = default;
	/**
	 * @brief Default copy assignation
	 * @return this
	 */
	CameraOrthoController &operator=(const CameraOrthoController &) = default;
	/**
	 * @brief Default move assignation
	 * @return this
	 */
	CameraOrthoController &operator=(CameraOrthoController &&) = default;
	/**
	 * @brief Default constructor.
	 */
	explicit CameraOrthoController(float aspectRatio, bool rotation = false);
	/**
	 * @brief Destructor.
	 */
	virtual ~CameraOrthoController() = default;

	/**
	 * @brief Frame function
	 * @param ts Time step
	 */
	void onUpdate(core::Timestep ts);
	/**
	 * @brief Event Management
	 * @param e The received event
	 */
	void onEvent(event::Event &e);
	/**
	 * @brief Action when view port is resized
	 * @param width New width
	 * @param height New height
	 */
	void onResize(float width, float height);
	/**
	 * @brief Access to the camera
	 * @return The camera
	 */
	renderer::CameraOrtho &getCamera() { return camera; }

	/**
	 * @brief Access to the camera
	 * @return The camera
	 */
	[[nodiscard]] const renderer::CameraOrtho &getCamera() const { return camera; }

	/**
	 * @brief Access to the zoom level
	 * @return Teh Zoom level
	 */
	[[nodiscard]] float getZoomLevel() const { return zoomLevel; }
	/**
	 * @brief Define the new zoom level
	 * @param level The new level
	 */
	void setZoomLevel(float level) { zoomLevel = level; }
private:
	/**
	 * @brief Action on mouse scroll
	 * @param e The Mouse event
	 * @return True if treated
	 */
	bool onMouseScrolled(event::MouseScrolledEvent &e);
	/**
	 * @brief Action on window resize
	 * @param e The Windo resize event
	 * @return True if treated
	 */
	bool onWindowResized(event::WindowResizeEvent &e);

	/// Aspect ratio of the camera
	float aspectRatio;
	/// Camera's zoom level
	float zoomLevel = 1.0f;
	/// The camera
	renderer::CameraOrtho camera;
	/// if camera's rotation allowed
	bool rotation;
	/// Camera's position in the world
	glm::vec3 cameraPosition = {0.0f, 0.0f, 0.0f};
	/// Camera's rotation in degrees, in the anti-clockwise direction
	float cameraRotation = 0.0f;
	/// Camera's Translation speed
	float cameraTranslationSpeed = 5.0f;
	/// Camera's Rotation Speed
	float cameraRotationSpeed = 180.0f;
};

}// namespace owl::input
