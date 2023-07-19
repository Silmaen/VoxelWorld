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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace owl::math {

constexpr float zero = 0.f;
constexpr float one = 1.f;
constexpr float epsi = glm::epsilon<float>();

glm::mat4 composeTransform(const glm::vec3 &translation, const glm::vec3 &rotation, const glm::vec3 &scale) {
	return glm::translate(glm::mat4(1.0f), translation) *
		   glm::rotate(glm::mat4(1.0f), rotation[2], {0, 0, 1}) *
		   glm::rotate(glm::mat4(1.0f), rotation[1], {0, 1, 0}) *
		   glm::rotate(glm::mat4(1.0f), rotation[0], {1, 0, 0}) *
		   glm::scale(glm::mat4(1.0f), scale);
}

bool decomposeTransform(const glm::mat4 &transform, glm::vec3 &translation, glm::vec3 &rotation, glm::vec3 &scale) {
	// From glm::decompose in matrix_decompose.inl

	glm::mat4 localMatrix(transform);
	// normalize the matrix
	if (glm::epsilonEqual(localMatrix[3][3], zero, epsi))
		return false;
	for (uint8_t i = 0; i < 4; ++i)
		for (uint8_t j = 0; j < 4; ++j)
			localMatrix[i][j] /= localMatrix[3][3];

	// first Suppress perspective.
	if (glm::epsilonNotEqual(localMatrix[0][3], zero, epsi) ||
		glm::epsilonNotEqual(localMatrix[1][3], zero, epsi) ||
		glm::epsilonNotEqual(localMatrix[2][3], zero, epsi)) {
		// clear the perspective part
		localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = zero;
		localMatrix[3][3] = one;
	}
	// Next take care of translation (easy).
	translation = glm::vec3(localMatrix[3]);
	localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

	glm::vec3 row[3];

	// Now get scale and shear.
	for (glm::length_t i = 0; i < 3; ++i)
		for (glm::length_t j = 0; j < 3; ++j)
			row[i][j] = localMatrix[i][j];
	// Compute X scale factor and normalize first row.
	scale.x = glm::length(row[0]);
	row[0] = glm::detail::scale(row[0], one);

	// Compute XY shear factor and make 2nd row orthogonal to 1st.
	row[1] = glm::detail::combine(row[1], row[0], one, -glm::dot(row[0], row[1]));

	// Now, compute Y scale and normalize 2nd row.
	scale.y = glm::length(row[1]);
	row[1] = glm::detail::scale(row[1], one);

	// Compute XZ and YZ shears, orthogonalize 3rd row.
	row[2] = glm::detail::combine(row[2], row[0], one, -glm::dot(row[0], row[2]));
	row[2] = glm::detail::combine(row[2], row[1], one, -glm::dot(row[1], row[2]));

	// Next, get Z scale and normalize 3rd row.
	scale.z = glm::length(row[2]);
	row[2] = glm::detail::scale(row[2], one);

	// At this point, the matrix (in rows[]) is orthonormal.
	// Check for a coordinate system flip.  If the determinant
	// is -1, then negate the matrix and the scaling factors.
	glm::vec3 pdum3 = glm::cross(row[1], row[2]);// v3Cross(row[1], row[2], Pdum3);
	if (glm::dot(row[0], pdum3) < 0) {
		for (glm::length_t i = 0; i < 3; i++) {
			scale[i] *= -one;
			row[i] *= -one;
		}
	}

	rotation.y = -glm::asin(row[0][2]);
	float cy = glm::cos(rotation.y);
	if (glm::epsilonNotEqual(cy, zero, epsi)) {
		rotation.x = math::atan2(row[1][2] / cy, row[2][2] / cy);
		rotation.z = math::atan2(row[0][1] / cy, row[0][0] / cy);
	} else {
		rotation.z = 0;
		rotation.x = math::atan2(-row[2][1], row[1][1]);
	}

	return true;
}

}// namespace owl::math
