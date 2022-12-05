/**
 * @file LayerStack.cpp
 * @author argawaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "LayerStack.h"
#include <algorithm>

namespace owl::core::layer {

LayerStack::~LayerStack() {
  for (auto &layer : layers) {
    layer->onDetach();
    layer.reset();
  }
}

void LayerStack::pushLayer(const shrd<Layer> &layer) {
  layers.emplace(layers.begin() + layerInsertIndex, layer);
  layerInsertIndex++;
}

void LayerStack::pushOverlay(const shrd<Layer> &overlay) {
  layers.emplace_back(overlay);
}

void LayerStack::popLayer(const shrd<Layer> &layer) {
  auto it = std::find(layers.begin(), layers.begin() + layerInsertIndex, layer);
  if (it != layers.begin() + layerInsertIndex) {
    layer->onDetach();
    layers.erase(it);
    layerInsertIndex--;
  }
}

void LayerStack::popOverlay(const shrd<Layer> &overlay) {
  auto it = std::find(layers.begin() + layerInsertIndex, layers.end(), overlay);
  if (it != layers.end()) {
    overlay->onDetach();
    layers.erase(it);
  }
}

} // namespace owl::core::layer
