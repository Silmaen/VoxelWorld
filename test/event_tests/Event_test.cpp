/**
 * @file Event_test.cpp
 * @author Silmaen
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "event/AppEvent.h"
#include "testHelper.h"

using namespace owl::event;

TEST(EventDispatcher, base) {
	AppTickEvent event;
	EventDispatcher dispatcher(event);
	EXPECT_FALSE(
			dispatcher.dispatch<AppUpdateEvent>([](Event &) { return true; }));
	EXPECT_TRUE(dispatcher.dispatch<AppTickEvent>([](Event &) { return false; }));
	EXPECT_FALSE(event.handled);
	EXPECT_TRUE(dispatcher.dispatch<AppTickEvent>([](Event &) { return true; }));
	EXPECT_TRUE(event.handled);
}
