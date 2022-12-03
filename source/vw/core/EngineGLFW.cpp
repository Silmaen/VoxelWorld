/**
 * @file EngineGLFW.cpp
 * @author damien.lachouette
 * @date 02/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "EngineGLFW.h"
#include "Tracker.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

namespace vw::core {

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int EngineGLFW::run() {
  auto ret = initialize();
  if (ret != 0)
    return ret;
  looping();
  finalize();
  return 0;
}

void EngineGLFW::finalize() {
  requestQuitLoop();
  glfwTerminate();
  auto globMem = Tracker::get().globals();
  spdlog::info("Finalizing game engine.");
  spdlog::info("Mem alloc calls  : {}", globMem.m_allocationCalls);
  spdlog::info("Mem dealloc calls: {}", globMem.m_deallocationCalls);
  spdlog::info("Remaining memory : {}", globMem.m_allocatedMemory);
  spdlog::info("Max memory usage : {}", globMem.m_memoryPeek);
  status = Status::Idle;
}

int EngineGLFW::initialize() {
  if (status != Status::Idle) {
    spdlog::info("Game engine already started, closing first...");
    finalize();
  }
  spdlog::info("Starting Game engine...");
  // Step 1: Initialize the glfw.
  if (!glfwInit()) {
    spdlog::error("Failed to initialize GLFW");
    return -1;
  }

  // Step 2: Create a window.
  window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test OpenGL", nullptr, nullptr);
  if (window == nullptr) {
    spdlog::error("Failed to create GLFW window");
    finalize();
    return -1;
  }

  // Step 3: create GL Context
  glfwMakeContextCurrent(window);

  // Step 4: initialize glew (to be defined after GL context!)
  if (glewInit() != GLEW_OK) {
    spdlog::error("Failed to initialize GLEW");
    return -1;
  }
  // Link a callback with frame size change.
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  spdlog::info("Engine started with OpenGL version {}",
               reinterpret_cast<const char *>(glGetString(GL_VERSION)));
  status = Status::Started;
  return 0;
}

void EngineGLFW::looping() {
  if (status != Status::Started) {
    spdlog::warn("Engine: try to loop while in bad state: {}",
                 magic_enum::enum_name(status));
    return;
  }

  float positions[] = {
      -0.5f, -0.5f,
       0.0f,  0.5f,
       0.5f, -0.5f};
  uint32_t buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  status = Status::Looping;
  // Start rendering infinitely.
  while (!glfwWindowShouldClose(window) && loopOk) {
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  status = Status::Ending;
}

void EngineGLFW::requestQuitLoop() {
  loopOk = false;
  while (status == Status::Looping) {
  }
  loopOk = true;
}

/** glfw: whenever the window size changed (by OS or user resize) this callback
 * function executes. */
void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width,
                               int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

/** Process all input: query GLFW whether relevant keys are pressed/released
 * this frame and react accordingly. */
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

} // namespace vw::core
