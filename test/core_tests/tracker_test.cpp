
#include "core/Tracker.h"
#include "testHelper.h"

using namespace owl::core;

TEST(Tracker, base){
  auto& track = Tracker::get();
  auto state = track.checkState();
  EXPECT_TRUE(state.m_allocationCalls==track.globals().m_allocationCalls);
}
