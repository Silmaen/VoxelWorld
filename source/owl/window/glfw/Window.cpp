/**
 * @file Window.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include <glad/glad.h>

#include "Window.h"
#include "core/Log.h"
#include "event/AppEvent.h"
#include "event/KeyEvent.h"
#include "event/MouseEvent.h"

namespace owl::window::glfw {

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char *description) {
  OWL_CORE_ERROR("GLFW Error ({}): {}", error, description);
}

Window::Window(const Properties &props) : ::owl::window::Window() {
  init(props);
}

Window::~Window() { shutdown(); }

void Window::onUpdate() {
  glfwPollEvents();
  glfwSwapBuffers(glfwWindow);
  // m_Context->SwapBuffers();
}
void Window::setVSync(bool enabled) {
  if (enabled)
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);

  windowData.VSync = enabled;
}
bool Window::isVSync() const { return windowData.VSync; }
void Window::init(const Properties &props) {
  windowData.title = props.Title;
  windowData.width = props.width;
  windowData.height = props.height;

  OWL_CORE_INFO("Creating window {} ({}, {})", props.Title, props.width,
                props.height);

  if (s_GLFWWindowCount == 0) {
    // OWL_PROFILE_SCOPE("glfwInit");
    [[maybe_unused]] int success = glfwInit();
    OWL_CORE_ASSERT(success, "Could not initialize GLFW!");
    glfwSetErrorCallback(GLFWErrorCallback);
  }

  {
    // OWL_PROFILE_SCOPE("glfwCreateWindow");
#if defined(OWL_DEBUG)
    // if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
    //   glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    glfwWindow = glfwCreateWindow(static_cast<int>(props.width),
                                  static_cast<int>(props.height),
                                  windowData.title.c_str(), nullptr, nullptr);
    ++s_GLFWWindowCount;
  }

  glfwMakeContextCurrent(glfwWindow);
  [[maybe_unused]] int status =
      gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
  OWL_CORE_ASSERT(status, "Failed to initialize GLAD");
  // m_Context = GraphicsContext::Create(glfwWindow);
  // m_Context->Init();

  glfwSetWindowUserPointer(glfwWindow, &windowData);
  setVSync(true);

  // Set GLFW callbacks
  glfwSetWindowSizeCallback(
      glfwWindow, [](GLFWwindow *window, int width, int height) {
        WindowData &data =
            *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
        data.width = static_cast<uint32_t>(width);
        data.height = static_cast<uint32_t>(height);

        event::WindowResizeEvent event(data.width, data.height);
        data.eventCallback(event);
      });

  glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow *window) {
    WindowData &data =
        *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    event::WindowCloseEvent event;
    data.eventCallback(event);
  });
  glfwSetKeyCallback(glfwWindow, [](GLFWwindow *window, int key,
                                    [[maybe_unused]] int scancode, int action,
                                    [[maybe_unused]] int mods) {
    WindowData &data =
        *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

    auto cKey = static_cast<input::KeyCode>(key);
    switch (action) {
    case GLFW_PRESS: {
      event::KeyPressedEvent event(cKey, false);
      data.eventCallback(event);
      break;
    }
    case GLFW_RELEASE: {
      event::KeyReleasedEvent event(cKey);
      data.eventCallback(event);
      break;
    }
    case GLFW_REPEAT: {
      event::KeyPressedEvent event(cKey, true);
      data.eventCallback(event);
      break;
    }
    default:
      break;
    }
  });

  glfwSetCharCallback(glfwWindow, [](GLFWwindow *window, unsigned int keycode) {
    WindowData &data =
        *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

    event::KeyTypedEvent event(static_cast<input::KeyCode>(keycode));
    data.eventCallback(event);
  });

  glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow *window, int button,
                                            int action,
                                            [[maybe_unused]] int mods) {
    WindowData &data =
        *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

    switch (action) {
    case GLFW_PRESS: {
      event::MouseButtonPressedEvent event(
          static_cast<input::MouseCode>(button));
      data.eventCallback(event);
      break;
    }
    case GLFW_RELEASE: {
      event::MouseButtonReleasedEvent event(
          static_cast<input::MouseCode>(button));
      data.eventCallback(event);
      break;
    }
    default:
      break;
    }
  });

  glfwSetScrollCallback(
      glfwWindow, [](GLFWwindow *window, double xOffset, double yOffset) {
        WindowData &data =
            *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

        event::MouseScrolledEvent event(static_cast<float>(xOffset),
                                        static_cast<float>(yOffset));
        data.eventCallback(event);
      });

  glfwSetCursorPosCallback(
      glfwWindow, [](GLFWwindow *window, double xPos, double yPos) {
        WindowData &data =
            *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
        event::MouseMovedEvent event(static_cast<float>(xPos),
                                     static_cast<float>(yPos));
        data.eventCallback(event);
      });
}
void Window::shutdown() {
  glfwDestroyWindow(glfwWindow);
  --s_GLFWWindowCount;

  if (s_GLFWWindowCount == 0) {
    glfwTerminate();
  }
}

} // namespace owl::window::glfw
