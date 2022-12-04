/**
 * @file EngineGLFW.cpp
 * @author damien.lachouette
 * @date 02/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "EngineGLFW.h"
#include "ResourceManager.h"
#include "Tracker.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <fstream>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>
#include <streambuf>
#include <debugbreak.h>

namespace owl::core {

/** glfw: whenever the window size changed (by OS or user resize) this callback
 * function executes. */
static void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window,
                                      int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

/** Process all input: query GLFW whether relevant keys are pressed/released
 * this frame and react accordingly. */
static void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

static std::string loadShader(const std::filesystem::path &file) {
  if (!exists(file)) {
    spdlog::warn("Shader {} does not exists.", file.string());
    return "";
  }
  std::ifstream stream(file.string());
  std::string str;
  stream.seekg(0, std::ios::end);
  str.reserve(stream.tellg());
  stream.seekg(0, std::ios::beg);
  str.assign((std::istreambuf_iterator<char>(stream)),
             std::istreambuf_iterator<char>());
  return str;
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static uint32_t CompileShader(uint32_t type, const std::string &sourceCode) {
  uint32_t id = glCreateShader(type);
  const char *src = sourceCode.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    std::string message;
    message.resize(length);
    glGetShaderInfoLog(id, length, &length, &message[0]);
    spdlog::error("CompileShader Fail compile {} shader:\n {}",
                  type == GL_VERTEX_SHADER
                      ? "vertex"
                      : (type == GL_FRAGMENT_SHADER ? "fragment" : "unknown"),
                  message);
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static uint32_t CreateShader(const std::string &vertexShader,
                             const std::string &fragmentShader) {
  uint32_t program = glCreateProgram();
  uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);
  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}

// ==============OpenGL error debug======================
static void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}
[[maybe_unused]]static bool GLLogCall(const char* function, const char* file, int line) {
  bool result = true;
  while (GLenum error = glGetError()) {
    spdlog::warn("[OpnGL Error] ({}) function: {} in {}:{}", error, function, file, line);
    result = false;
  }
  return result;
}

#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
    debug_break();
#define GLCall(x)                                                              \
  owl::core::GLClearError();                                                    \
  x;                                                                           \
  ASSERT(owl::core::GLLogCall(#x, __FILE__, __LINE__))
// ==============OpenGL error debug======================
EngineGLFW::EngineGLFW() { ResourceManager::get().findResource(); }

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

  uint32_t shader;
  int location;
  {
    float positions[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};
    uint32_t indices[] = {0, 1, 2, 2, 3, 0};

    uint32_t buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    uint32_t ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(uint32_t), indices,
                 GL_STATIC_DRAW);

    std::string vertexShader =
        loadShader(ResourceManager::get().getShaderPath() / "basic.vert");
    std::string fragmentShader =
        loadShader(ResourceManager::get().getShaderPath() / "basic.frag");
    shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    GLCall(location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.2f,0.3f,0.8f,1.0f));
  }

  status = Status::Looping;
  float r = 0.2f;
  float increm = 0.01f;
  // Start rendering infinitely.
  while (!glfwWindowShouldClose(window) && loopOk) {
    processInput(window);

    r += increm;
    if (r>=0.8f || r<=0.2f)increm*=-1.f;
    glClear(GL_COLOR_BUFFER_BIT);

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    GLCall(glUniform4f(location, r,0.3f,0.8f,1.0f));
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(shader);
  status = Status::Ending;
}

void EngineGLFW::requestQuitLoop() {
  loopOk = false;
  while (status == Status::Looping) {
  }
  loopOk = true;
}
} // namespace owl::core
