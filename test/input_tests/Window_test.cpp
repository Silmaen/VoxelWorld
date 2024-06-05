
#include "testHelper.h"

#include <window/Window.h>

using namespace owl::window;

TEST(Window, creation) {
    owl::core::Log::init(spdlog::level::off);
    Properties props;
    props.winType = Type(-1);
    auto wnd = Window::create(props);
    EXPECT_FALSE(wnd);
    props.winType = Type::Null;
    wnd = Window::create(props);
    ASSERT_TRUE(wnd);
    EXPECT_EQ(wnd->getType(), Type::Null);
    EXPECT_EQ(wnd->getHeight(), 900);
    EXPECT_EQ(wnd->getWidth(), 1600);
    EXPECT_EQ(wnd->getNativeWindow(), nullptr);
    EXPECT_TRUE(wnd->isVSync());
    wnd->setVSync(false);
    EXPECT_FALSE(wnd->isVSync());
    owl::core::Log::invalidate();
}
