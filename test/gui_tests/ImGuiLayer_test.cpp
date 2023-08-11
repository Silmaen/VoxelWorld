
#include "testHelper.h"

#include <event/KeyEvent.h>
#include <gui/UILayer.h>

using namespace owl::gui;

TEST(ImGuiLayer, creation) {
	UILayer layer;
	EXPECT_STREQ(layer.getName().c_str(), "ImGuiLayer");
	layer.blockEvents(false);
	owl::event::KeyPressedEvent evt(owl::input::key::A, 1);
	layer.onEvent(evt);
}
