/**
 * @file Input.h
 * @author argawaen
 * @date 06/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "input/Input.h"

namespace owl::input::glfw {

/**
 * @brief GLFW implementation of Class Input
 */
class Input: public ::owl::input::Input {
public:
  Input(const Input &) = delete;
  Input(Input &&) = delete;
  Input &operator=(const Input &) = delete;
  Input &operator=(Input &&) = delete;
  /**
   * @brief Default constructor.
   */
  Input();
  /**
   * @brief Destructor.
   */
  virtual ~Input() = default;

protected:
  /**
   * @brief Implementation  of keyboard pressed check
   * @param keycode The Key to check
   * @return True if pressed
   */
  bool isKeyPressedImpl(KeyCode keycode) override;

  /**
   * @brief Implementation of mouse button pressed check
   * @param mouseCode Mouse button to check
   * @return True if pressed
   */
  bool isMouseButtonPressedImpl(MouseCode mouseCode) override;

  /**
   * @brief Implementation of get mouse X position
   * @return Mouse X Position
   */
  float getMouseXImpl()override;
  /**
   * @brief Implementation of get mouse Y position
   * @return Mouse Y Position
   */
  float getMouseYImpl() override;
  /**
   * @brief Get mouse position
   * @return Mouse Position
   */
  glm::vec2 getMousePosImpl() override;

};

} // namespace owl
