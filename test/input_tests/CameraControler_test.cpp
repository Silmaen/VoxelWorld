
#include "testHelper.h"

#include <input/CameraOrthoController.h>

using namespace owl::input;

TEST(Input, baseCameraControler) {
	CameraOrthoController coc(1);
	EXPECT_EQ(coc.getZoomLevel(), 1.0f);
	coc.setZoomLevel(2.0f);
	EXPECT_EQ(coc.getZoomLevel(), 2.0f);
}

TEST(Input, ResizeCameraControler) {
	CameraOrthoController coc(1);
	coc.onResize(10, 5);
	EXPECT_EQ(coc.getZoomLevel(), 1.0f);
}

TEST(Input, EventCameraControler) {
	CameraOrthoController coc(1);
	owl::event::AppRenderEvent e;
	coc.onEvent(e);// nothing
	owl::event::WindowResizeEvent re(10, 5);
	coc.onEvent(re);// resize
	owl::event::MouseScrolledEvent me(0, 1);
	coc.onEvent(me);// resize
	EXPECT_EQ(coc.getZoomLevel(), 0.75f);
}
