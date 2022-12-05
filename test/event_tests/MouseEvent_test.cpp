/**
 * @file MouseEvent_test.cpp
 * @author damien.lachouette
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "event/MouseEvent.h"
#include "testHelper.h"

using namespace owl::event;

TEST(MouseEvent, ButtonPressed) {
  MouseButtonPressedEvent event(12);
  EXPECT_STREQ(event.getName().c_str(), "MouseButtonPressedEvent");
  EXPECT_EQ(event.getType(), type::MouseButtonPressed);
  EXPECT_EQ(event.getStaticType(), type::MouseButtonPressed);
  EXPECT_EQ(event.getCategoryFlags(),
            category::Input | category::Mouse | category::MouseButton);
  EXPECT_STREQ(event.toString().c_str(), "MouseButtonPressedEvent: 12");
  EXPECT_TRUE(event.isinCategory(category::Input));
  EXPECT_FALSE(event.isinCategory(category::Keyboard));
  EXPECT_FALSE(event.isinCategory(category::Application));
  EXPECT_TRUE(event.isinCategory(category::Mouse));
  EXPECT_TRUE(event.isinCategory(category::MouseButton));
  EXPECT_FALSE(event.isinCategory(category::None));
}

TEST(MouseEvent, ButtonReleased) {
  MouseButtonReleasedEvent event(12);
  EXPECT_STREQ(event.getName().c_str(), "MouseButtonReleasedEvent");
  EXPECT_EQ(event.getType(), type::MouseButtonReleased);
  EXPECT_EQ(event.getStaticType(), type::MouseButtonReleased);
  EXPECT_EQ(event.getCategoryFlags(),
            category::Input | category::Mouse | category::MouseButton);
  EXPECT_STREQ(event.toString().c_str(), "MouseButtonReleasedEvent: 12");
  EXPECT_TRUE(event.isinCategory(category::Input));
  EXPECT_FALSE(event.isinCategory(category::Keyboard));
  EXPECT_FALSE(event.isinCategory(category::Application));
  EXPECT_TRUE(event.isinCategory(category::Mouse));
  EXPECT_TRUE(event.isinCategory(category::MouseButton));
  EXPECT_FALSE(event.isinCategory(category::None));
}

TEST(MouseEvent, Moved) {
  MouseMovedEvent event(144, 155);
  EXPECT_STREQ(event.getName().c_str(), "MouseMovedEvent");
  EXPECT_EQ(event.getType(), type::MouseMoved);
  EXPECT_EQ(event.getStaticType(), type::MouseMoved);
  EXPECT_EQ(event.getCategoryFlags(), category::Input | category::Mouse);
  EXPECT_EQ(event.getX(), 144);
  EXPECT_EQ(event.getY(), 155);
  EXPECT_STREQ(event.toString().c_str(), "MouseMovedEvent: 144, 155");
  EXPECT_TRUE(event.isinCategory(category::Input));
  EXPECT_FALSE(event.isinCategory(category::Keyboard));
  EXPECT_FALSE(event.isinCategory(category::Application));
  EXPECT_TRUE(event.isinCategory(category::Mouse));
  EXPECT_FALSE(event.isinCategory(category::MouseButton));
  EXPECT_FALSE(event.isinCategory(category::None));
}

TEST(MouseEvent, Scrolled) {
  MouseScrolledEvent event(1, 1);
  EXPECT_STREQ(event.getName().c_str(), "MouseScrolledEvent");
  EXPECT_EQ(event.getType(), type::MouseScrolled);
  EXPECT_EQ(event.getStaticType(), type::MouseScrolled);
  EXPECT_EQ(event.getCategoryFlags(), category::Input | category::Mouse);
  EXPECT_EQ(event.getXOff(), 1);
  EXPECT_EQ(event.getYOff(), 1);
  EXPECT_STREQ(event.toString().c_str(), "MouseScrolledEvent: 1, 1");
  EXPECT_TRUE(event.isinCategory(category::Input));
  EXPECT_FALSE(event.isinCategory(category::Keyboard));
  EXPECT_FALSE(event.isinCategory(category::Application));
  EXPECT_TRUE(event.isinCategory(category::Mouse));
  EXPECT_FALSE(event.isinCategory(category::MouseButton));
  EXPECT_FALSE(event.isinCategory(category::None));
}
