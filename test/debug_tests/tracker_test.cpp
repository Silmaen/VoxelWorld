
#include "testHelper.h"

#include <debug/Tracker.h>

using namespace owl::debug;

TEST(Tracker, base) {
	auto &track = Tracker::get();
	const auto state = track.checkState();
	EXPECT_TRUE(state.allocationCalls <= track.globals().allocationCalls);
	ASSERT_FALSE(state.allocs.empty());
	const auto s = state.allocs.back().toStr();
	ASSERT_FALSE(s.empty());
}
