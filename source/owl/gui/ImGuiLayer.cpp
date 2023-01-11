/**
 * @file ImGuiLayer.cpp
 * @author Silmaen
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "ImGuiLayer.h"
#include "core/Application.h"
#include "core/external/glfw3.h"
#include "core/external/imgui.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#include <ImGuizmo.h>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace owl::gui {

ImGuiLayer::ImGuiLayer() : core::layer::Layer("ImGuiLayer") {}
ImGuiLayer::~ImGuiLayer() = default;

void ImGuiLayer::onAttach() {// Setup Dear ImGui context
	OWL_PROFILE_FUNCTION()

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |=
			ImGuiConfigFlags_NavEnableKeyboard;        // Enable Keyboard Controls
													   // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad
													   // Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;// Enable Multi-Viewport /
													   // Platform Windows
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	// Better fonts
	auto assetDir = core::Application::get().getAssetDirectory();
	float fontsize = 18.0f;
	if (exists(assetDir / "fonts" / "opensans" / "OpenSans-Bold.ttf")) {
		io.Fonts->AddFontFromFileTTF((assetDir / "fonts" / "opensans" / "OpenSans-Bold.ttf").string().c_str(), fontsize);
	} else {
		OWL_CORE_WARN("Unable to find OpenSans-Bold, fall back to default font")
	}
	if (exists(assetDir / "fonts" / "opensans" / "OpenSans-Regular.ttf")) {
		io.FontDefault = io.Fonts->AddFontFromFileTTF((assetDir / "fonts" / "opensans" / "OpenSans-Regular.ttf").string().c_str(), fontsize);
	} else {
		OWL_CORE_WARN("Unable to find OpenSans-Regular, fall back to default font")
	}

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform
	// windows can look identical to regular ones.
	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	setDarkThemeColors();

	auto *window = static_cast<GLFWwindow *>(
			core::Application::get().getWindow().getNativeWindow());
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::onDetach() {
	OWL_PROFILE_FUNCTION()

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::onEvent([[maybe_unused]] event::Event &event) {
	if (blockEvent) {
		ImGuiIO &io = ImGui::GetIO();
		event.handled |= event.isInCategory(event::category::Mouse) & io.WantCaptureMouse;
		event.handled |= event.isInCategory(event::category::Keyboard) & io.WantCaptureKeyboard;
	}
}

void ImGuiLayer::begin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	if (dockingEnable) {
		initializeDocking();
	}
}

void ImGuiLayer::end() {
	OWL_PROFILE_FUNCTION()
	if (dockingEnable) {
		ImGui::End();
	}
	ImGuiIO &io = ImGui::GetIO();
	core::Application &app = core::Application::get();
	io.DisplaySize = ImVec2(static_cast<float>(app.getWindow().getWidth()),
							static_cast<float>(app.getWindow().getHeight()));
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow *backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiLayer::setDarkThemeColors() {
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

void ImGuiLayer::initializeDocking() {
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen) {
		ImGuiViewport *viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;
	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();
	if (opt_fullscreen)
		ImGui::PopStyleVar(2);
	// DockSpace
	ImGuiIO &io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	style.WindowMinSize.x = minWinSizeX;
}

}// namespace owl::gui
