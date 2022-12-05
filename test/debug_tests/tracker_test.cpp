
#include "debug/Tracker.h"
#include "testHelper.h"

using namespace owl::debug;

TEST(Tracker, base){
  auto& track = Tracker::get();
  auto state = track.checkState();
  EXPECT_TRUE(state.allocationCalls ==track.globals().allocationCalls);
}
