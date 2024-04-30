
#include "testHelper.h"

#include <core/Log.h>

using namespace owl::core;

TEST(Log, basic) {
	Log::setVerbosityLevel(spdlog::level::off);
	EXPECT_EQ(Log::getClientLogger(), nullptr);
	EXPECT_EQ(Log::getCoreLogger(), nullptr);
	Log::init();
	Log::init();
	Log::newFrame();
	EXPECT_NE(Log::getClientLogger(), nullptr);
	EXPECT_NE(Log::getCoreLogger(), nullptr);
	Log::setFrameFrequency(0);
	Log::newFrame();
	Log::setFrameFrequency(1);
	Log::newFrame();
	Log::invalidate();
}
