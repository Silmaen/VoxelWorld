/**
 * @file ImGuiLayer.cpp
 * @author damien.lachouette
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "ImGuiLayer.h"
#include "core/Application.h"
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

namespace owl::gui {

ImGuiLayer::ImGuiLayer() : core::layer::Layer("ImGuiLayer") {}
ImGuiLayer::~ImGuiLayer() = default;

void ImGuiLayer::onAttach() { // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad
  // Controls io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         //
  // Enable Docking io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       //
  // Enable Multi-Viewport / Platform Windows io.ConfigFlags |=
  // ImGuiConfigFlags_ViewportsNoTaskBarIcons; io.ConfigFlags |=
  // ImGuiConfigFlags_ViewportsNoMerge;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  SetDarkThemeColors();

  auto *window = static_cast<GLFWwindow *>(
      core::Application::get().getWindow()->getNativeWindow());
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::onDetach() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::onUpdate() {
  ImGuiIO &io = ImGui::GetIO();
  core::Application &app = core::Application::get();
  io.DisplaySize = ImVec2(static_cast<float>(app.getWindow()->getWidth()),
                          static_cast<float>(app.getWindow()->getHeight()));

  float time = static_cast<float>(glfwGetTime());
  io.DeltaTime = savedTime > 0 ? (time - savedTime) : (1.f / 60.f);
  savedTime = time;

  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  static bool show = true;
  ImGui::ShowDemoWindow(&show);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::onEvent(event::Event &event) {
  if (blockEvents) {
    ImGuiIO &io = ImGui::GetIO();
    event.handled |=
        event.isInCategory(event::category::Mouse) & io.WantCaptureMouse;
    event.handled |=
        event.isInCategory(event::category::Keyboard) & io.WantCaptureKeyboard;
  }
}

void ImGuiLayer::SetDarkThemeColors() {
  auto &colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

  // Headers
  colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Buttons
  colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Frame BG
  colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Tabs
  colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
  colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
  colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

  // Title
  colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
}

} // namespace owl::gui
