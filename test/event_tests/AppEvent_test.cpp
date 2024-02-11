/**
 * @file AppEvent_test.cpp
 * @author Silmaen
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "testHelper.h"

#include <event/AppEvent.h>

using namespace owl::event;

TEST(AppEvent, Tick) {
	const AppTickEvent event;
	EXPECT_STREQ(event.getName().c_str(), "AppTickEvent");
	EXPECT_EQ(event.getType(), type::AppTick);
	EXPECT_EQ(event.getStaticType(), type::AppTick);
	EXPECT_EQ(event.getCategoryFlags(), category::Application);
	EXPECT_STREQ(event.toString().c_str(), "AppTickEvent");
	EXPECT_FALSE(event.isInCategory(category::Input));
	EXPECT_FALSE(event.isInCategory(category::Keyboard));
	EXPECT_TRUE(event.isInCategory(category::Application));
	EXPECT_FALSE(event.isInCategory(category::Mouse));
	EXPECT_FALSE(event.isInCategory(category::MouseButton));
	EXPECT_FALSE(event.isInCategory(category::None));
}

TEST(AppEvent, Update) {
	const AppUpdateEvent event;
	EXPECT_STREQ(event.getName().c_str(), "AppUpdateEvent");
	EXPECT_EQ(event.getType(), type::AppUpdate);
	EXPECT_EQ(event.getStaticType(), type::AppUpdate);
	EXPECT_EQ(event.getCategoryFlags(), category::Application);
	EXPECT_STREQ(event.toString().c_str(), "AppUpdateEvent");
	EXPECT_FALSE(event.isInCategory(category::Input));
	EXPECT_FALSE(event.isInCategory(category::Keyboard));
	EXPECT_TRUE(event.isInCategory(category::Application));
	EXPECT_FALSE(event.isInCategory(category::Mouse));
	EXPECT_FALSE(event.isInCategory(category::MouseButton));
	EXPECT_FALSE(event.isInCategory(category::None));
}

TEST(AppEvent, Render) {
	const AppRenderEvent event;
	EXPECT_STREQ(event.getName().c_str(), "AppRenderEvent");
	EXPECT_EQ(event.getType(), type::AppRender);
	EXPECT_EQ(event.getStaticType(), type::AppRender);
	EXPECT_EQ(event.getCategoryFlags(), category::Application);
	EXPECT_STREQ(event.toString().c_str(), "AppRenderEvent");
	EXPECT_FALSE(event.isInCategory(category::Input));
	EXPECT_FALSE(event.isInCategory(category::Keyboard));
	EXPECT_TRUE(event.isInCategory(category::Application));
	EXPECT_FALSE(event.isInCategory(category::Mouse));
	EXPECT_FALSE(event.isInCategory(category::MouseButton));
	EXPECT_FALSE(event.isInCategory(category::None));
}

TEST(WindowEvent, Close) {
	const WindowCloseEvent event;
	EXPECT_STREQ(event.getName().c_str(), "WindowCloseEvent");
	EXPECT_EQ(event.getType(), type::WindowClose);
	EXPECT_EQ(event.getStaticType(), type::WindowClose);
	EXPECT_EQ(event.getCategoryFlags(), category::Application);
	EXPECT_STREQ(event.toString().c_str(), "WindowCloseEvent");
	EXPECT_FALSE(event.isInCategory(category::Input));
	EXPECT_FALSE(event.isInCategory(category::Keyboard));
	EXPECT_TRUE(event.isInCategory(category::Application));
	EXPECT_FALSE(event.isInCategory(category::Mouse));
	EXPECT_FALSE(event.isInCategory(category::MouseButton));
	EXPECT_FALSE(event.isInCategory(category::None));
}

TEST(WindowEvent, Resize) {
	const WindowResizeEvent event(1422, 166);
	EXPECT_STREQ(event.getName().c_str(), "WindowResizeEvent");
	EXPECT_EQ(event.getType(), type::WindowResize);
	EXPECT_EQ(event.getStaticType(), type::WindowResize);
	EXPECT_EQ(event.getCategoryFlags(), category::Application);
	EXPECT_STREQ(event.toString().c_str(), "WindowResizeEvent: 1422, 166");
	EXPECT_EQ(event.getWidth(), 1422);
	EXPECT_EQ(event.getHeight(), 166);
	EXPECT_FALSE(event.isInCategory(category::Input));
	EXPECT_FALSE(event.isInCategory(category::Keyboard));
	EXPECT_TRUE(event.isInCategory(category::Application));
	EXPECT_FALSE(event.isInCategory(category::Mouse));
	EXPECT_FALSE(event.isInCategory(category::MouseButton));
	EXPECT_FALSE(event.isInCategory(category::None));
}
