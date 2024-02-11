
#include "testHelper.h"

#include <cmath>
#include <math/Transform.h>

using namespace owl::math;

constexpr bool vecNear(const glm::vec3 &a, const glm::vec3 &b, float accuracy = 0.001f) {
	return (std::abs(a[0] - b[0]) < accuracy) &&
		   (std::abs(a[1] - b[1]) < accuracy) &&
		   (std::abs(a[2] - b[2]) < accuracy);
}

const float pis2 = 2.f * std::atan(1.f);

TEST(math, decomposeTrivial) {
	glm::mat4 mat{};
	glm::vec3 tran{};
	glm::vec3 rot{};
	glm::vec3 scale{};
	// null matrix
	EXPECT_FALSE(decomposeTransform(mat, tran, rot, scale));

	// identity matrix
	mat[0][0] = 1;
	mat[1][1] = 1;
	mat[2][2] = 1;
	mat[3][3] = 1;
	EXPECT_TRUE(decomposeTransform(mat, tran, rot, scale));
	EXPECT_TRUE(vecNear(tran, {0, 0, 0}));
	EXPECT_TRUE(vecNear(rot, {0, 0, 0}));
	EXPECT_TRUE(vecNear(scale, {1, 1, 1}));

	// pure scale matrix
	mat[0][0] = 0.1f;
	mat[1][1] = 0.1f;
	mat[2][2] = 0.1f;
	mat[3][3] = 1;
	EXPECT_TRUE(decomposeTransform(mat, tran, rot, scale));
	EXPECT_TRUE(vecNear(tran, {0, 0, 0}));
	EXPECT_TRUE(vecNear(rot, {0, 0, 0}));
	EXPECT_TRUE(vecNear(scale, {0.1, 0.1, 0.1}));
}

TEST(math, decomposeWithPerspective) {
	glm::mat4 mat{};
	glm::vec3 tran{};
	glm::vec3 rot{};
	glm::vec3 scale{};
	mat[0][0] = 1;
	mat[1][1] = 1;
	mat[2][2] = 1;
	mat[3][3] = 1;
	// first perspective
	mat[0][3] = 0.1f;
	mat[1][3] = 0.;
	mat[2][3] = 0.;
	EXPECT_TRUE(decomposeTransform(mat, tran, rot, scale));
	EXPECT_TRUE(vecNear(tran, {0, 0, 0}));
	EXPECT_TRUE(vecNear(rot, {0, 0, 0}));
	EXPECT_TRUE(vecNear(scale, {1, 1, 1}));
	mat[0][3] = 0.;
	mat[1][3] = 0.1f;
	mat[2][3] = 0.;
	EXPECT_TRUE(decomposeTransform(mat, tran, rot, scale));
	EXPECT_TRUE(vecNear(tran, {0, 0, 0}));
	EXPECT_TRUE(vecNear(rot, {0, 0, 0}));
	EXPECT_TRUE(vecNear(scale, {1, 1, 1}));
	mat[0][3] = 0.;
	mat[1][3] = 0.;
	mat[2][3] = 0.1f;
	EXPECT_TRUE(decomposeTransform(mat, tran, rot, scale));
	EXPECT_TRUE(vecNear(tran, {0, 0, 0}));
	EXPECT_TRUE(vecNear(rot, {0, 0, 0}));
	EXPECT_TRUE(vecNear(scale, {1, 1, 1}));
}

TEST(math, composeDecompose) {

	const std::vector<std::array<glm::vec3, 3>> data = {
			{glm::vec3{0.f, 0.f, 0.f}, glm::vec3{pis2, 0.f, 0.f}, glm::vec3{1.f, 1.f, 1.f}},
			{glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, pis2, 0.f}, glm::vec3{1.f, 1.f, 1.f}},
			{glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 0.f, pis2}, glm::vec3{1.f, 1.f, 1.f}},
			{glm::vec3{10.f, -0.2f, 0.7f}, glm::vec3{-pis2 / 2.f, 0.f, pis2}, glm::vec3{1.1f, 0.4f, 1.8f}},
			{glm::vec3{1.f, 0.0f, 0.0f}, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{-1.0f, -1.0f, -1.0f}}};

	for (const auto &ele: data) {
		glm::mat4 mat{};
		glm::vec3 tran{};
		glm::vec3 rot{};
		glm::vec3 scale{};
		mat = composeTransform(ele[0], ele[1], ele[2]);
		EXPECT_TRUE(decomposeTransform(mat, tran, rot, scale));
		EXPECT_TRUE(vecNear(tran, ele[0]));
		EXPECT_TRUE(vecNear(rot, ele[1]));
		EXPECT_TRUE(vecNear(scale, ele[2]));
	}
}
