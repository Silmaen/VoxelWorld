
#include "testHelper.h"

#include <debug/TrackerClient.h>

using namespace owl::debug;

TEST(Tracker, base) {
	const auto state = TrackerAPI::checkState();
	EXPECT_TRUE(state.allocationCalls <= TrackerAPI::globals().allocationCalls);
	ASSERT_FALSE(state.allocs.empty());
	const auto s = state.allocs.back().toStr();
	ASSERT_FALSE(s.empty());
}

TEST(Tracker, stacktrace) {
	owl::core::Log::init(spdlog::level::off);
	{
		const auto& globals = TrackerAPI::globals();
		const size_t initialAlloc = globals.allocs.size();
		EXPECT_EQ(globals.allocationCalls - globals.deallocationCalls, initialAlloc);
		{
			auto mat = owl::mkShared<owl::math::mat2>();
			EXPECT_EQ(globals.allocs.size(), initialAlloc + 1);
			EXPECT_FALSE(globals.allocs.back().toStr(false, true).empty());
			mat.reset();
			EXPECT_LT(std::abs(static_cast<int64_t>(globals.allocs.size()) - static_cast<int64_t>(initialAlloc)), 2);
		}
		EXPECT_LT(std::abs(static_cast<int64_t>(globals.allocationCalls) -
						   static_cast<int64_t>(globals.deallocationCalls) - static_cast<int64_t>(initialAlloc)),
				  2);
		EXPECT_LT(std::abs(static_cast<int64_t>(globals.allocs.size()) - static_cast<int64_t>(initialAlloc)), 2);
	}
}
