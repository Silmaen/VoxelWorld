
#include "testHelper.h"

#include <core/Core.h>

using namespace owl;

TEST(Core, ptr) {
	const auto toto_uniq = mkUniq<int32_t>(3);
	const auto toto_shared = mkShared<int32_t>(3);
	EXPECT_EQ(*toto_uniq, *toto_shared);
}
