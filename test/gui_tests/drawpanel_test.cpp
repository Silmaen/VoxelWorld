
#include "testHelper.h"

#include <gui/BaseDrawPanel.h>
#include <gui/UiLayer.h>

using namespace owl::gui;
using namespace owl::core;

TEST(BaseDrawPanel, basic) {
	Log::init(spdlog::level::off);
	UiLayer layer;
	layer.disableApp();
	layer.enableDocking();
	layer.onAttach();
	BaseDrawPanel drawPanel;
	drawPanel.setName("superpanel");
	EXPECT_STREQ(drawPanel.getName().c_str(), "superpanel");
	const Timestep ts;
	drawPanel.onUpdate(ts);
	layer.begin();
	drawPanel.onRender();
	layer.end();
	layer.onDetach();
	Log::invalidate();
}
