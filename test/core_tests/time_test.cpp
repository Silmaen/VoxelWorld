
#include "testHelper.h"
#include <core/Timestep.h>

using namespace owl::core;

TEST(TimeStep, update) {
	Timestep ts;
	EXPECT_LT(ts.getSeconds(), 0.001);   // there can be a little delay.
	EXPECT_LT(ts.getMilliseconds(), 1.0);// there can be a little delay.
	ts.update();
	EXPECT_GT(ts.getFps(), 0);
}
