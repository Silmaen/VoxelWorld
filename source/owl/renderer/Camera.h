/**
 * @file Camera.h
 * @author Silmaen
 * @date 23/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

namespace owl::renderer {
/**
 * @brief Class Camera.
 */
class OWL_API Camera {
public:
	Camera(const Camera&) = default;
	Camera(Camera&&) = default;
	Camera& operator=(const Camera&) = default;
	Camera& operator=(Camera&&) = default;
	Camera() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~Camera();

	/**
	 * @brief Construct the camera with a projection.
	 * @param[in] iProjection projection matrix.
	 */
	explicit Camera(const glm::mat4& iProjection) : m_projection{iProjection} {}

	/**
	 * @brief Get the projection matrix of the camera.
	 * @return The camera's projection matrix.
	 */
	[[nodiscard]] const glm::mat4& getProjection() const { return m_projection; }

protected:
	/// Camera's projection.
	glm::mat4 m_projection = glm::mat4(1.0f);
};
} // namespace owl::renderer
