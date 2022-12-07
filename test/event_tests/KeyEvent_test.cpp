/**
 * @file KeyEvent_test.cpp
 * @author Silmaen
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "event/KeyEvent.h"
#include "testHelper.h"

using namespace owl::event;

TEST(KeyEvent, Pressed) {
  KeyPressedEvent event(12);
  EXPECT_STREQ(event.getName().c_str(), "KeyPressedEvent");
  EXPECT_EQ(event.getType(), type::KeyPressed);
  EXPECT_EQ(event.getStaticType(), type::KeyPressed);
  EXPECT_FALSE(event.isRepeated());
  EXPECT_EQ(event.getCategoryFlags(), category::Input | category::Keyboard);
  EXPECT_STREQ(event.toString().c_str(),
               "KeyPressedEvent: 12 (repeat = false)");
  EXPECT_TRUE(event.isInCategory(category::Input));
  EXPECT_TRUE(event.isInCategory(category::Keyboard));
  EXPECT_FALSE(event.isInCategory(category::Application));
  EXPECT_FALSE(event.isInCategory(category::Mouse));
  EXPECT_FALSE(event.isInCategory(category::MouseButton));
  EXPECT_FALSE(event.isInCategory(category::None));
}

TEST(KeyEvent, Typed) {
  KeyTypedEvent event(12);
  EXPECT_STREQ(event.getName().c_str(), "KeyTypedEvent");
  EXPECT_EQ(event.getType(), type::KeyTyped);
  EXPECT_EQ(event.getStaticType(), type::KeyTyped);
  EXPECT_EQ(event.getCategoryFlags(), category::Input | category::Keyboard);
  EXPECT_STREQ(event.toString().c_str(), "KeyTypedEvent: 12");
  EXPECT_TRUE(event.isInCategory(category::Input));
  EXPECT_TRUE(event.isInCategory(category::Keyboard));
  EXPECT_FALSE(event.isInCategory(category::Application));
  EXPECT_FALSE(event.isInCategory(category::Mouse));
  EXPECT_FALSE(event.isInCategory(category::MouseButton));
  EXPECT_FALSE(event.isInCategory(category::None));
}

TEST(KeyEvent, Released) {
  KeyReleasedEvent event(12);
  EXPECT_STREQ(event.getName().c_str(), "KeyReleasedEvent");
  EXPECT_EQ(event.getType(), type::KeyReleased);
  EXPECT_EQ(event.getStaticType(), type::KeyReleased);
  EXPECT_EQ(event.getCategoryFlags(), category::Input | category::Keyboard);
  EXPECT_STREQ(event.toString().c_str(), "KeyReleasedEvent: 12");
  EXPECT_TRUE(event.isInCategory(category::Input));
  EXPECT_TRUE(event.isInCategory(category::Keyboard));
  EXPECT_FALSE(event.isInCategory(category::Application));
  EXPECT_FALSE(event.isInCategory(category::Mouse));
  EXPECT_FALSE(event.isInCategory(category::MouseButton));
  EXPECT_FALSE(event.isInCategory(category::None));
}
