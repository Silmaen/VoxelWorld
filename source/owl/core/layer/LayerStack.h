/**
 * @file LayerStack.h
 * @author argawaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Layer.h"
#include <vector>

namespace owl::core::layer {

/**
 * @brief Class LayerStack
 */
class LayerStack {
public:
  using layerList = std::vector<shrd<Layer>>;
  using iterator = layerList::iterator;
  using const_iterator = layerList::const_iterator;
  using reverse_iterator = layerList::reverse_iterator;
  using const_reverse_iterator = layerList::const_reverse_iterator;
  LayerStack(const LayerStack &) = delete;
  LayerStack(LayerStack &&) = delete;
  LayerStack &operator=(const LayerStack &) = delete;
  LayerStack &operator=(LayerStack &&) = delete;
  /**
   * @brief Default constructor.
   */
  LayerStack() = default;
  /**
   * @brief Destructor.
   */
  virtual ~LayerStack();

  void pushLayer(const shrd<Layer> &layer);
  void pushOverlay(const shrd<Layer> &overlay);
  void popLayer(const shrd<Layer> &layer);
  void popOverlay(const shrd<Layer> &overlay);

  [[nodiscard]] iterator begin() { return layers.begin(); }
  [[nodiscard]] iterator end() { return layers.end(); }
  [[nodiscard]] reverse_iterator rbegin() { return layers.rbegin(); }
  [[nodiscard]] reverse_iterator rend() { return layers.rend(); }

  [[nodiscard]] const_iterator begin() const { return layers.begin(); }
  [[nodiscard]] const_iterator end() const { return layers.end(); }
  [[nodiscard]] const_reverse_iterator rbegin() const {
    return layers.rbegin();
  }
  [[nodiscard]] const_reverse_iterator rend() const { return layers.rend(); }

private:
  layerList layers;
  unsigned int layerInsertIndex = 0;
};

} // namespace owl::core::layer
