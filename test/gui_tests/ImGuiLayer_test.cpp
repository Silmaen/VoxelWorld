
#include "testHelper.h"

#include <event/KeyEvent.h>
#include <gui/UiLayer.h>

using namespace owl::gui;

TEST(ImGuiLayer, creation) {
	UiLayer layer;
	EXPECT_STREQ(layer.getName().c_str(), "ImGuiLayer");
	layer.blockEvents(false);
	owl::event::KeyPressedEvent evt(owl::input::key::A, 1);
	layer.onEvent(evt);
}
