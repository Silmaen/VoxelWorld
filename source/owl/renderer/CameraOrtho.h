/**
 * @file CameraOrtho.h
 * @author Silmaen
 * @date 10/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/Core.h"
#include "core/external/glm.h"

namespace owl::renderer {

/**
 * @brief Orthographic camera
 */
class OWL_API CameraOrtho {
public:
	/**
	 * @brief Default copy constructor
	 */
	CameraOrtho(const CameraOrtho &) = default;
	/**
	 * @brief Default move constructor
	 */
	CameraOrtho(CameraOrtho &&) = default;
	/**
	 * @brief Default copy assignation
	 * @return this
	 */
	CameraOrtho &operator=(const CameraOrtho &) = default;
	/**
	 * @brief Default move assignation
	 * @return this
	 */
	CameraOrtho &operator=(CameraOrtho &&) = default;
	/**
	 * @brief Create the camera giving coordinates of corners
	 * @param left Left of the screen's coordinate.
	 * @param right Right of the screen's coordinate.
	 * @param bottom Bottom of the screen's coordinate.
	 * @param top Top of the screen's coordinate.
	 */
	CameraOrtho(float left, float right, float bottom, float top);

	/**
	 * @brief Set projection giving the camera coordinates of corners
	 * @param left Left of the screen's coordinate.
	 * @param right Right of the screen's coordinate.
	 * @param bottom Bottom of the screen's coordinate.
	 * @param top Top of the screen's coordinate.
	 */
	void setProjection(float left, float right, float bottom, float top);
	/**
	 * @brief Access to camera's position
	 * @return Camera's position
	 */
	[[nodiscard]] const glm::vec3& getPosition() const { return m_Position; }
	/**
	 * @brief Define camera's position
	 * @param position New camera position
	 */
	void setPosition(const glm::vec3& position) { m_Position = position; recalculateViewMatrix(); }

	/**
	 * @brief Access to camera's rotation
	 * @return Camera's rotation
	 */
	[[nodiscard]] float getRotation() const { return m_Rotation; }
	/**
	 * @brief Defines camera's rotation
	 * @param rotation New camera rotation
	 */
	void setRotation(float rotation) { m_Rotation = rotation; recalculateViewMatrix(); }
	/**
	 * @brief Access to projection matrix
	 * @return The projection matrix
	 */
	[[nodiscard]] const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
	/**
	 * @brief Access to view matrix
	 * @return The view matrix
	 */
	[[nodiscard]] const glm::mat4& getViewMatrix() const { return viewMatrix; }
	/**
	 * @brief Access to view projection matrix
	 * @return The view position matrix
	 */
	[[nodiscard]] const glm::mat4& getViewProjectionMatrix() const { return viewProjectionMatrix; }
private:
	/**
	 * @brief Recompute the matrices
	 */
	void recalculateViewMatrix();

	/// The projection matrix
	glm::mat4 projectionMatrix;
	/// The view matrix
	glm::mat4 viewMatrix;
	/// The view projection matrix
	glm::mat4 viewProjectionMatrix;
	/// Camera's position
	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	/// Camera's rotation
	float m_Rotation = 0.0f;
};

}// namespace owl::renderer
