/**
 * @file LayerStack_test.cpp
 * @author Silmaen
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "core/layer/LayerStack.h"
#include "testHelper.h"

using namespace owl::core::layer;

TEST(LayerStack, base) {
  {
    LayerStack stack;
    EXPECT_EQ(stack.begin(), stack.end());
    EXPECT_EQ(stack.rbegin(), stack.rend());
  }
  {
    const LayerStack stack;
    EXPECT_EQ(stack.begin(), stack.end());
    EXPECT_EQ(stack.rbegin(), stack.rend());
  }
}

TEST(LayerStack, pushLayer) {
  {
    LayerStack stack;
    stack.pushLayer(owl::mk_shrd<Layer>("Layer 1"));
    stack.pushLayer(owl::mk_shrd<Layer>("Layer 2"));
    stack.pushOverlay(owl::mk_shrd<Layer>("Overlay 1"));
    stack.pushLayer(owl::mk_shrd<Layer>("Layer 3"));
  }
  {
    LayerStack stack;
    stack.pushLayer(owl::mk_shrd<Layer>("Layer 1"));
    stack.pushLayer(owl::mk_shrd<Layer>("Layer 2"));
    stack.pushOverlay(owl::mk_shrd<Layer>("Overlay 1"));
    stack.pushLayer(owl::mk_shrd<Layer>("Layer 3"));
    auto item = *(stack.begin() + 2);
    EXPECT_STREQ(item->getName().c_str(), "Layer 3");
    auto item2 = *(stack.begin() + 3);
    EXPECT_STREQ(item2->getName().c_str(), "Overlay 1");
  }
  {
    LayerStack stack;
    stack.pushLayer(owl::mk_shrd<Layer>("Layer 1"));
    stack.pushLayer(owl::mk_shrd<Layer>("Layer 2"));
    stack.pushOverlay(owl::mk_shrd<Layer>("Overlay 1"));
    stack.pushLayer(owl::mk_shrd<Layer>("Layer 3"));
    auto item = *(stack.begin() + 2);
    auto item2 = *(stack.begin() + 3);
    stack.popLayer(item);
    stack.popOverlay(item2);
    stack.popLayer(item);
    stack.popOverlay(item2);
    EXPECT_EQ(std::distance(stack.begin(), stack.end()), 2);
  }
}
