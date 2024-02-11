
#include "testHelper.h"

#include <math/sizingTypes.h>

using namespace owl::math;

TEST(math, sizingType_base) {
	FrameSize size{3, 4};
	EXPECT_EQ(size.getWidth(), 3);
	EXPECT_EQ(size.getHeight(), 4);
	EXPECT_EQ(size.surface(), 12);
	const FrameSize s2{size};
	EXPECT_TRUE(s2 == size);
	EXPECT_FALSE(s2 != size);
	size.height() += 2;
	EXPECT_FALSE(s2 == size);
	EXPECT_TRUE(s2 != size);
	size.width() += 4;
	EXPECT_EQ(size.getWidth(), 7);
	EXPECT_EQ(size.getHeight(), 6);
	EXPECT_EQ(size.surface(), 42);
	EXPECT_EQ(size.at(0), size[0]);
	EXPECT_EQ(size.at(1), size[1]);
	EXPECT_EQ(size.at(23), size[0]);

	EXPECT_FALSE(s2 == size);
	EXPECT_TRUE(s2 != size);
	EXPECT_EQ(s2.at(0), s2[0]);
	EXPECT_EQ(s2.at(1), s2[1]);
	EXPECT_EQ(s2.at(23), s2[0]);
}

TEST(math, sizingType_OPERATOR) {
	FrameSize size{3, 4};
	size += {2, 1};
	EXPECT_EQ(size[0], 5);
	EXPECT_EQ(size[1], 5);
	const FrameSize s2 = FrameSize{34, 45} - 0.5 * size * 4 + FrameSize{1, 2};
	EXPECT_EQ(s2[0], 23);
	EXPECT_EQ(s2[1], 35);
	size = s2 + FrameSize{};
	EXPECT_EQ(size[0], 23);
	EXPECT_EQ(size[1], 35);
	{
		FrameSize s{std::move(size)};
		s += {4, 3};
		size = std::move(s);
	}
	EXPECT_EQ(size[0], 27);
	EXPECT_EQ(size[1], 38);
}
