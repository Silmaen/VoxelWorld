
#include "testHelper.h"

#include <core/Timestep.h>

using namespace owl::core;

TEST(TimeStep, update) {
	Timestep ts;
#ifdef OWL_PLATFORM_ARM64
	const float millis = 10.f;// due to some emulation slowness.
#elif defined(OWL_STACKTRACE)
	float const millis = 4.f;
#else
	float const millis = 1.f;
#endif
	EXPECT_LT(ts.getSeconds(), 0.001f * millis);// there can be a little delay.
	EXPECT_LT(ts.getMilliseconds(), millis);// there can be a little delay.
	EXPECT_EQ(ts.getFrameNumber(), 1);
	ts.update();
	EXPECT_GT(ts.getFps(), 0);
	EXPECT_EQ(ts.getFrameNumber(), 2);
	EXPECT_GT(ts.getStabilizedFps(), 0);
}
