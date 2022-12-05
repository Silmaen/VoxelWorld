/**
 * @file Window.h
 * @author damien.lachouette
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "window/Window.h"

namespace owl::window::null {

/**
 * @brief Class for Null Window
 */
class OWL_API Window : public ::owl::window::Window {
public:
  Window(const Window &) = delete;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = delete;
  Window &operator=(Window &&) = delete;
  /**
   * @brief Default constructor.
   * @param props The window properties
   */
  explicit Window(const Properties &props);

  /**
   * @brief Destructor.
   */
  ~Window() override;
  /**
   * @brief Function called at Update Time
   */
  void onUpdate() override;

  /**
   * @brief Get Size attribute of width
   * @return The window's width
   */
  [[nodiscard]] uint32_t getWidth() const override { return 0; }

  /**
   * @brief Get Size attribute of height
   * @return The window's height
   */
  [[nodiscard]] uint32_t getHeight() const override { return 0; }

  /**
   * @brief Define the Event Callback function
   * @param callback The new callback function
   */
  void
  setEventCallback([[maybe_unused]] const EventCallback &callback) override {}
  /**
   * @brief St the VSync
   * @param enabled Should VSync enabled
   */
  void setVSync(bool enabled) override;
  /**
   * @brief Check for VSync
   * @return True if VSync enabled
   */
  [[nodiscard]] bool isVSync() const override;

  /**
   * @brief Access to the Native Window
   * @return Native window's raw pointer
   */
  [[nodiscard]] void *getNativeWindow() const override { return nullptr; }

private:
};

} // namespace owl::window::null
