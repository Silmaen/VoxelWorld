/**
 * @file Transform.cpp
 * @author Silmaen
 * @date 30/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Transform.h"
#include "math/trigonometry.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace owl::math {

constexpr float g_zero = 0.f;
constexpr float g_one = 1.f;
constexpr float g_epsi = glm::epsilon<float>();

glm::mat4 composeTransform(const glm::vec3 &iTranslation, const glm::vec3 &iRotation, const glm::vec3 &iScale) {
	return glm::translate(glm::mat4(1.0f), iTranslation) *
	       glm::rotate(glm::mat4(1.0f), iRotation[2], {0, 0, 1}) *
	       glm::rotate(glm::mat4(1.0f), iRotation[1], {0, 1, 0}) *
	       glm::rotate(glm::mat4(1.0f), iRotation[0], {1, 0, 0}) *
	       glm::scale(glm::mat4(1.0f), iScale);
}

bool decomposeTransform(const glm::mat4 &iTransform, glm::vec3 &oTranslation, glm::vec3 &oRotation, glm::vec3 &oScale) {
	// From glm::decompose in matrix_decompose.inl

	glm::mat4 localMatrix(iTransform);
	// normalize the matrix
	if (glm::epsilonEqual(localMatrix[3][3], g_zero, g_epsi))
		return false;
	for (uint8_t i = 0; i < 4; ++i) {
		for (uint8_t j = 0; j < 4; ++j)
			localMatrix[i][j] /= localMatrix[3][3];
	}
	// first Suppress perspective.
	if (glm::epsilonNotEqual(localMatrix[0][3], g_zero, g_epsi) ||
	    glm::epsilonNotEqual(localMatrix[1][3], g_zero, g_epsi) ||
	    glm::epsilonNotEqual(localMatrix[2][3], g_zero, g_epsi)) {
		// clear the perspective part
		localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = g_zero;
		localMatrix[3][3] = g_one;
	}
	// Next take care of translation (easy).
	oTranslation = glm::vec3(localMatrix[3]);
	localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

	glm::vec3 row[3];

	// Now get scale and shear.
	for (glm::length_t i = 0; i < 3; ++i) {
		for (glm::length_t j = 0; j < 3; ++j)
			row[i][j] = localMatrix[i][j];
	}
	// Compute X scale factor and normalize first row.
	oScale.x = glm::length(row[0]);
	row[0] = glm::detail::scale(row[0], g_one);

	// Compute XY shear factor and make 2nd row orthogonal to 1st.
	row[1] = glm::detail::combine(row[1], row[0], g_one, -glm::dot(row[0], row[1]));

	// Now, compute Y scale and normalize 2nd row.
	oScale.y = glm::length(row[1]);
	row[1] = glm::detail::scale(row[1], g_one);

	// Compute XZ and YZ shears, orthogonalize 3rd row.
	row[2] = glm::detail::combine(row[2], row[0], g_one, -glm::dot(row[0], row[2]));
	row[2] = glm::detail::combine(row[2], row[1], g_one, -glm::dot(row[1], row[2]));

	// Next, get Z scale and normalize 3rd row.
	oScale.z = glm::length(row[2]);
	row[2] = glm::detail::scale(row[2], g_one);

	// At this point, the matrix (in rows[]) is orthonormal.
	// Check for a coordinate system flip.  If the determinant
	// is -1, then negate the matrix and the scaling factors.
	if (const glm::vec3 pdum3 = glm::cross(row[1], row[2]); glm::dot(row[0], pdum3) < 0) {
		for (glm::length_t i = 0; i < 3; i++) {
			oScale[i] *= -g_one;
			row[i] *= -g_one;
		}
	}

	oRotation.y = -glm::asin(row[0][2]);

	if (const float cy = glm::cos(oRotation.y); glm::epsilonNotEqual(cy, g_zero, g_epsi)) {
		oRotation.x = math::atan2(row[1][2] / cy, row[2][2] / cy);
		oRotation.z = math::atan2(row[0][1] / cy, row[0][0] / cy);
	} else {
		oRotation.z = 0;
		oRotation.x = math::atan2(-row[2][1], row[1][1]);
	}

	return true;
}

}// namespace owl::math
