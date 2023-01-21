/**
 * @file Layer_test.cpp
 * @author Silmaen
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "core/layer/Layer.h"
#include "event/AppEvent.h"
#include "testHelper.h"

using namespace owl::core;
using namespace owl::event;
using namespace owl::core::layer;

TEST(Layer, base) {
	Layer layer("test");
	owl::core::Timestep ts;
	ts.update();
	layer.onUpdate(ts);
	layer.onAttach();
	layer.onDetach();
	owl::event::AppTickEvent evt;
	layer.onEvent(evt);
	EXPECT_STREQ(layer.getName().c_str(), "test");
}
