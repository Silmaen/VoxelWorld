/**
 * @file UILayer.cpp
 * @author Silmaen
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "UILayer.h"
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

namespace {
inline ImVec4 glm2im(const glm::vec4 &vec) {
	return ImVec4(vec.x, vec.y, vec.z, vec.w);
}
}// namespace

#include "Roboto-Bold.embed"
#include "Roboto-Italic.embed"
#include "Roboto-Regular.embed"

UILayer::UILayer() : core::layer::Layer("ImGuiLayer") {}
UILayer::~UILayer() = default;

void UILayer::onAttach() {// Setup Dear ImGui context
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
	ImFontConfig fontConfig;
	fontConfig.FontDataOwnedByAtlas = false;
	ImFont *robotoFont = io.Fonts->AddFontFromMemoryTTF(const_cast<void *>(static_cast<const void *>(g_RobotoRegular)), sizeof(g_RobotoRegular), 20.0f, &fontConfig);
	io.Fonts->AddFontFromMemoryTTF(const_cast<void *>(static_cast<const void *>(g_RobotoBold)), sizeof(g_RobotoBold), 20.0f, &fontConfig);
	io.Fonts->AddFontFromMemoryTTF(const_cast<void *>(static_cast<const void *>(g_RobotoItalic)), sizeof(g_RobotoItalic), 20.0f, &fontConfig);
	io.FontDefault = robotoFont;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform
	// windows can look identical to regular ones.
	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	setTheme();

	auto *window = static_cast<GLFWwindow *>(
			core::Application::get().getWindow().getNativeWindow());
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	if (renderer::RenderAPI::getAPI() == renderer::RenderAPI::Type::OpenGL)
		ImGui_ImplOpenGL3_Init("#version 410");
	if (renderer::RenderAPI::getAPI() == renderer::RenderAPI::Type::OpenglLegacy)
		ImGui_ImplOpenGL2_Init();
}

void UILayer::onDetach() {
	OWL_PROFILE_FUNCTION()

	if (renderer::RenderAPI::getAPI() == renderer::RenderAPI::Type::OpenGL)
		ImGui_ImplOpenGL3_Shutdown();
	if (renderer::RenderAPI::getAPI() == renderer::RenderAPI::Type::OpenglLegacy)
		ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UILayer::onEvent([[maybe_unused]] event::Event &event) {
	if (blockEvent) {
		ImGuiIO &io = ImGui::GetIO();
		event.handled |= event.isInCategory(event::category::Mouse) & io.WantCaptureMouse;
		event.handled |= event.isInCategory(event::category::Keyboard) & io.WantCaptureKeyboard;
	}
}

void UILayer::begin() {
	if (renderer::RenderAPI::getAPI() == renderer::RenderAPI::Type::OpenGL)
		ImGui_ImplOpenGL3_NewFrame();
	if (renderer::RenderAPI::getAPI() == renderer::RenderAPI::Type::OpenglLegacy)
		ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	if (dockingEnable) {
		initializeDocking();
	}
}

void UILayer::end() {
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
	if (renderer::RenderAPI::getAPI() == renderer::RenderAPI::Type::OpenGL)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (renderer::RenderAPI::getAPI() == renderer::RenderAPI::Type::OpenglLegacy)
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow *backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void UILayer::setTheme(const Theme &theme) {

	auto &style = ImGui::GetStyle();
	auto &colors = ImGui::GetStyle().Colors;

	// ======================
	// Colors

	// Headers
	colors[ImGuiCol_Header] = glm2im(theme.groupHeader);
	colors[ImGuiCol_HeaderHovered] = glm2im(theme.groupHeader);
	colors[ImGuiCol_HeaderActive] = glm2im(theme.groupHeader);

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{0.22f, 0.22f, 0.22f, 0.784f};
	colors[ImGuiCol_ButtonHovered] = ImVec4{0.275f, 0.275f, 0.275f, 1.0f};
	colors[ImGuiCol_ButtonActive] = ImVec4{0.22f, 0.22f, 0.22f, 0.588f};

	// Frame BG
	colors[ImGuiCol_FrameBg] = glm2im(theme.propertyField);
	colors[ImGuiCol_FrameBgHovered] = glm2im(theme.propertyField);
	colors[ImGuiCol_FrameBgActive] = glm2im(theme.propertyField);

	// Tabs
	colors[ImGuiCol_Tab] = glm2im(theme.titleBar);
	colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
	colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
	colors[ImGuiCol_TabUnfocused] = glm2im(theme.titleBar);
	colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_TabHovered];

	// Title
	colors[ImGuiCol_TitleBg] = glm2im(theme.titleBar);
	colors[ImGuiCol_TitleBgActive] = glm2im(theme.titleBar);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

	// Resize Grip
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

	// Scrollbar
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

	// Check Mark
	colors[ImGuiCol_CheckMark] = ImColor(200, 200, 200, 255);

	// Slider
	colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);

	// Text
	colors[ImGuiCol_Text] = glm2im(theme.text);

	// Checkbox
	colors[ImGuiCol_CheckMark] = glm2im(theme.text);

	// Separator
	colors[ImGuiCol_Separator] = glm2im(theme.backgroundDark);
	colors[ImGuiCol_SeparatorActive] = glm2im(theme.highlight);
	colors[ImGuiCol_SeparatorHovered] = ImColor(39, 185, 242, 150);

	// Window Background
	colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};
	colors[ImGuiCol_ChildBg] = glm2im(theme.background);
	colors[ImGuiCol_PopupBg] = glm2im(theme.backgroundPopup);
	colors[ImGuiCol_Border] = glm2im(theme.backgroundDark);

	// Tables
	colors[ImGuiCol_TableHeaderBg] = glm2im(theme.groupHeader);
	colors[ImGuiCol_TableBorderLight] = glm2im(theme.backgroundDark);

	// Menubar
	colors[ImGuiCol_MenuBarBg] = ImVec4{0.0f, 0.0f, 0.0f, 0.0f};

	//========================================================
	// Style
	style.FrameRounding = 2.5f;
	style.FrameBorderSize = 1.0f;
	style.IndentSpacing = 11.0f;
}

void UILayer::initializeDocking() {
	static bool dockSpaceOpen = true;
	static bool optFullScreenPersistant = true;
	bool optFullScreen = optFullScreenPersistant;
	static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;
	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (optFullScreen) {
		ImGuiViewport *viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		windowFlags |= ImGuiWindowFlags_NoBackground;
	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dock space/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockSpaceOpen, windowFlags);
	ImGui::PopStyleVar();
	if (optFullScreen)
		ImGui::PopStyleVar(2);
	// DockSpace
	ImGuiIO &io = ImGui::GetIO();
	ImGuiStyle &style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockSpaceId = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);
	}
	style.WindowMinSize.x = minWinSizeX;
}

}// namespace owl::gui
