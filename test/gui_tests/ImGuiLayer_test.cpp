
#include "testHelper.h"

#include <event/KeyEvent.h>
#include <gui/ImGuiLayer.h>

using namespace owl::gui;

TEST(ImGuiLayer, creation) {
	ImGuiLayer layer;
	EXPECT_STREQ(layer.getName().c_str(), "ImGuiLayer");
	layer.blockEvents(false);
	owl::event::KeyPressedEvent evt(owl::input::key::A, 1);
	layer.onEvent(evt);
}
