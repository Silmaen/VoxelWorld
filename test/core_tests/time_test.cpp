
#include "testHelper.h"

#include <core/Timestep.h>

using namespace owl::core;

TEST(TimeStep, update) {
	Timestep ts;
#ifdef OWL_PLATFORM_ARM64
	float millis = 10.f; // due to some emulation slowness.
#else
	float millis = 1.f;
#endif
	EXPECT_LT(ts.getSeconds(), 0.001f * millis);// there can be a little delay.
	EXPECT_LT(ts.getMilliseconds(), millis);// there can be a little delay.
	ts.update();
	EXPECT_GT(ts.getFps(), 0);
}
