/**
 * @file EngineGLFW.h
 * @author damien.lachouette
 * @date 02/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <memory>

class GLFWwindow;

namespace owl::core {

/**
 * @brief Class EngineGLFW
 */
class EngineGLFW {
public:
  EngineGLFW(const EngineGLFW &) = delete;
  EngineGLFW(EngineGLFW &&) = delete;
  EngineGLFW &operator=(const EngineGLFW &) = delete;
  EngineGLFW &operator=(EngineGLFW &&) = delete;
  /**
   * @brief Default constructor.
   */
  EngineGLFW();
  /**
   * @brief Destructor.
   */
  virtual ~EngineGLFW() = default; //---UNCOVER---

  /**
   * @brief Main entry point of the game engine
   * @return Exec Return code
   */
  int run();

  int initialize();
  /**
   * @brief Possible engine status
   */
  enum class Status {
    Idle,    /// Waiting fo orders
    Started, /// Ready to loop
    Looping, /// During main loop
    Ending,  /// after looping
  };
  [[nodiscard]] const Status &getStatus() const { return status; }

private:
  /// The current status of the engine
  Status status = Status::Idle;
  /// Pointer to the glfw window
  GLFWwindow *window = nullptr;
  /// If the Mainloop can go on.
  bool loopOk = true;

  void looping();

  void finalize();

  void requestQuitLoop();
};
} // namespace owl::core
