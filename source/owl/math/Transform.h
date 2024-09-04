/**
 * @file Transform.h
 * @author Silmaen
 * @date 30/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "matrices.h"

namespace owl::math {

/**
 * @brief Compose transformation's matrix from vector components.
 * @param[in] iTranslation Translation vector.
 * @param[in] iRotation Rotation vector in radian.
 * @param[in] iScale Scale vector.
 * @return Transformation matrix.
 */
OWL_API auto composeTransform(const vec3& iTranslation, const vec3& iRotation, const vec3& iScale) -> mat4;

/**
 * @brief Decompose transformation's matrix into vector components.
 * @param[in] iTransform Transformation matrix.
 * @param[out] oTranslation Translation vector.
 * @param[out] oRotation Rotation vector in radian.
 * @param[out] oScale Scale vector.
 * @return True if decomposition succeed.
 */
OWL_API auto decomposeTransform(const mat4& iTransform, vec3& oTranslation, vec3& oRotation, vec3& oScale) -> bool;

}// namespace owl::math
