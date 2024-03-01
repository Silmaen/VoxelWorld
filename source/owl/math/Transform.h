/**
 * @file Transform.h
 * @author Silmaen
 * @date 30/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

namespace owl::math {

/**
 * @brief Compose transformation's matrix from vector components.
 * @param[in] iTranslation Translation vector.
 * @param[in] iRotation Rotation vector in radian.
 * @param[in] iScale Scale vector.
 * @return Transformation matrix.
 */
OWL_API glm::mat4 composeTransform(const glm::vec3 &iTranslation, const glm::vec3 &iRotation, const glm::vec3 &iScale);

/**
 * @brief Decompose transformation's matrix into vector components.
 * @param[in] iTransform Transformation matrix.
 * @param[out] oTranslation Translation vector.
 * @param[out] oRotation Rotation vector in radian.
 * @param[out] oScale Scale vector.
 * @return True if decomposition succeed.
 */
OWL_API bool decomposeTransform(const glm::mat4 &iTransform, glm::vec3 &oTranslation, glm::vec3 &oRotation,
                                glm::vec3 &oScale);

}// namespace owl::math
