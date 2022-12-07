/**
 * @file Layer.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <utility>

#include "core/Core.h"
#include "event/Event.h"

/**
 * @brief Namespace for the layer definition
 */
namespace owl::core::layer {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class Layer
 */
class OWL_API Layer {
public:
  Layer(const Layer &) = delete;
  Layer(Layer &&) = delete;
  Layer &operator=(const Layer &) = delete;
  Layer &operator=(Layer &&) = delete;
  /**
   * @brief Default constructor.
   */
  explicit Layer(std::string name = "Layer") : debugName(std::move(name)) {}
  /**
   * @brief Destructor.
   */
  virtual ~Layer() = default;

  /**
   * @brief Action on Attach
   */
  virtual void onAttach() {}
  /**
   * @brief Action on detach
   */
  virtual void onDetach() {}
  /**
   * @brief Action on update
   */
  virtual void onUpdate() {}
  /**
   * @brief Action for that layer when gui is rendered
   */
  virtual void onImGuiRender() {}
  /**
   * @brief Action on event
   * @param event The Event to react
   */
  virtual void onEvent([[maybe_unused]] event::Event &event) {}

  /**
   * @brief Get the debug name of the layer
   * @return Debug Name of the layer
   */
  [[nodiscard]] const std::string &getName() const { return debugName; }

protected:
  /// Debug name for this layer
  std::string debugName;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

} // namespace owl::core::layer
