/**
 * @file Transform.h
 * @author Silmaen
 * @date 30/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"
#include <glm/glm.hpp>

namespace owl::math {

/**
 * @brief Decompose transformation's matrix into vector components
 * @param[in] transform Transformation matrix
 * @param[out] translation Translation vector
 * @param[out] rotation Rotation vector
 * @param[out] scale Scale vector
 * @return True if decomposition succeed
 */
OWL_API bool decomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

}
