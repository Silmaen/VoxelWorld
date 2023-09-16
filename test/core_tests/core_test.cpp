
#include "testHelper.h"
#include <core/Core.h>

using namespace owl;

TEST(Core, ptr) {
	auto toto_uniq = mk_uniq<int32_t>(3);
	auto toto_shared = mk_shared<int32_t>(3);
	EXPECT_EQ(*toto_uniq, *toto_shared);
}