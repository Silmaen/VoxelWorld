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

bool decomposeTransform(const glm::mat4 &transform, glm::vec3 &translation, glm::vec3 &rotation, glm::vec3 &scale) {
	// From glm::decompose in matrix_decompose.inl

	glm::mat4 localMatrix(transform);
	// normalize the matrix
	if (glm::epsilonEqual(localMatrix[3][3], zero, epsi))
		return false;

	// first Isolate perspective. this is the messiest
	if (glm::epsilonEqual(localMatrix[0][3], zero, epsi) ||
		glm::epsilonEqual(localMatrix[1][3], zero, epsi) ||
		glm::epsilonEqual(localMatrix[2][3], zero, epsi)) {
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
	scale.y = glm::length(row[1]);
	row[1] = glm::detail::scale(row[1], one);
	scale.z = glm::length(row[2]);
	row[2] = glm::detail::scale(row[2], one);

	// At this point, the matrix (in rows[]) is orthonormal.
	// Check for a coordinate system flip.  If the determinant
	// is -1, then negate the matrix and the scaling factors.
#if 0
		glm::vec3 pdum3 = glm::cross(row[1], row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (glm::dot(row[0], pdum3) < 0) {
			for (glm::length_t i = 0; i < 3; i++) {
				scale[i] *= -one;
				row[i] *= -one;
			}
		}
#endif

	rotation.y = glm::asin(-row[0][2]);
	if (glm::epsilonEqual(glm::cos(rotation.y), zero, epsi)) {
		rotation.x = math::atan2(row[1][2], row[2][2]);
		rotation.z = math::atan2(row[0][1], row[0][0]);
	} else {
		rotation.x = math::atan2(-row[2][0], row[1][1]);
		rotation.z = 0;
	}


	return true;
}

}// namespace owl::math
