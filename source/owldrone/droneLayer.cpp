/**
 * @file droneLayer.cpp
 * @author Silmaen
 * @date 16/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "droneLayer.h"
#include "event/KeyEvent.h"
#include <imgui.h>

using namespace owl;

namespace drone {


droneLayer::droneLayer() : core::layer::Layer("droneLayer") {
}

void droneLayer::onAttach() {
	OWL_PROFILE_FUNCTION()
	core::Application::get().enableDocking();

	renderer::FramebufferSpecification specs;
	specs.attachments = {
			renderer::FramebufferTextureFormat::RGBA8,
			renderer::FramebufferTextureFormat::RED_INTEGER,
			renderer::FramebufferTextureFormat::Depth};
	specs.width = 1280;
	specs.height = 720;
	framebuffer = renderer::Framebuffer::create(specs);

	auto iconPath = core::Application::get().getAssetDirectory() / "icons";
	iconExit = renderer::Texture2D::create(iconPath / "exit.png");
	iconGauges = renderer::Texture2D::create(iconPath / "gauges.png");
	iconSettings = renderer::Texture2D::create(iconPath / "settings.png");
	iconConnected = renderer::Texture2D::create(iconPath / "connected.png");
	iconConnect = renderer::Texture2D::create(iconPath / "connect.png");
}

void droneLayer::onDetach() {
	OWL_PROFILE_FUNCTION()

	iconExit.reset();
	iconGauges.reset();
	iconSettings.reset();
	iconConnected.reset();
	iconConnect.reset();
	framebuffer.reset();
}

void droneLayer::onUpdate(const core::Timestep &) {
	OWL_PROFILE_FUNCTION()
	auto spec = framebuffer->getSpecification();
	auto width = static_cast<uint32_t>(viewportSize.x);
	auto height = static_cast<uint32_t>(viewportSize.y);
	if (width > 0 && height > 0 && (width != spec.width || height != spec.height)) {
		framebuffer->resize(width, height);
	}
	// Render
	framebuffer->bind();
	renderer::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
	renderer::RenderCommand::clear();

	// Clear our entity ID attachment to -1
	framebuffer->clearAttachment(1, -1);

	// Do the drawings!
	// ===============================================================

	// ===============================================================
	// free the frame buffer.
	framebuffer->unbind();
}

void droneLayer::onEvent(event::Event &event) {

	event::EventDispatcher dispatcher(event);
	dispatcher.dispatch<event::KeyPressedEvent>([this](auto &&PH1) {
		return onKeyPressed(std::forward<decltype(PH1)>(PH1));
	});
	dispatcher.dispatch<event::MouseButtonPressedEvent>([this](auto &&PH1) {
		return onMouseButtonPressed(std::forward<decltype(PH1)>(PH1));
	});
}

void droneLayer::onImGuiRender(const core::Timestep &ts) {
	OWL_PROFILE_FUNCTION()

	// ==================================================================
	if (showStats)
		renderStats(ts);
	//=============================================================
	renderMenu();
	//============================================================
	switch (mode) {
		case DisplayMode::Settings:
			renderSettings();
			break;
		case DisplayMode::Gauges:
			renderViewport();
			renderGauges();
			renderMisc();
			break;
	}
	// ===== Toolbar: last, to be on top ========================
	renderToolbar();
}

void droneLayer::renderStats(const core::Timestep &ts) {
	auto &tracker = debug::Tracker::get();
	ImGui::Begin("Stats");
	ImGui::Text("%s", fmt::format("FPS: {:.2f}", ts.getFps()).c_str());
	ImGui::Separator();
	ImGui::Text("%s", fmt::format("Current used memory: {}",
								  tracker.globals().allocatedMemory)
							  .c_str());
	ImGui::Text("%s", fmt::format("Max used memory: {}", tracker.globals().memoryPeek)
							  .c_str());
	ImGui::Text(
			"%s", fmt::format("Allocation calls: {}", tracker.globals().allocationCalls)
						  .c_str());
	ImGui::Text("%s", fmt::format("Deallocation calls: {}",
								  tracker.globals().deallocationCalls)
							  .c_str());
	ImGui::Separator();
	ImGui::Separator();
	auto stats = renderer::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());
	ImGui::End();
}

void droneLayer::renderMenu() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) owl::core::Application::get().close();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Debug")) {
			ImGui::MenuItem("Show Stats", nullptr, &showStats, true);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

bool droneLayer::onKeyPressed(event::KeyPressedEvent &e) {
	// Shortcuts
	if (e.getRepeatCount() > 0)
		return false;

	// return non-blocking.
	return false;
}

bool droneLayer::onMouseButtonPressed(event::MouseButtonPressedEvent &e) {
	if (e.GetMouseButton() == input::mouse::ButtonLeft) {
		return false;
	}

	// return non-blocking.
	return false;
}

void droneLayer::renderToolbar() {
	ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	auto &colors = ImGui::GetStyle().Colors;
	const auto &buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
	const auto &buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));


	float size = ImGui::GetWindowHeight() * .95f;
	float padding = ImGui::GetWindowHeight() * .025f;
	float posX = padding;
	ImVec2 vsize = ImVec2(size, size);

	ImGui::SetCursorPos(ImVec2(posX, padding));
	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(iconSettings->getRendererID()), vsize, ImVec2(0, 0), ImVec2(1, 1), 0)) {
		mode = DisplayMode::Settings;
	}
	posX += size + 2.f * padding;
	ImGui::SetCursorPos(ImVec2(posX, padding));
	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(iconGauges->getRendererID()), vsize, ImVec2(0, 1), ImVec2(1, 0), 0)) {
		mode = DisplayMode::Gauges;
	}
	/*
	posX += size + 2.f * padding;
	ImGui::SetCursorPos(ImVec2(posX, padding));
	if (ImGui::Button("btn3", vsize)) {
	}
*/
	shared<renderer::Texture2D> iconCC = isConnected() ? iconConnected : iconConnect;
	ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionMax().x) - (2.f * size + 3.f * padding), padding));
	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(iconCC->getRendererID()), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0)) {
		toggleConnect();
	}
	ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionMax().x) - (size + padding), padding));
	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(iconExit->getRendererID()), vsize, ImVec2(0, 1), ImVec2(1, 0), 0)) {
		owl::core::Application::get().close();
	}
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
	ImGui::End();
}

void droneLayer::renderViewport() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Drone Viewport");
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	viewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
	viewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

	viewportFocused = ImGui::IsWindowFocused();
	viewportHovered = ImGui::IsWindowHovered();
	core::Application::get().getImGuiLayer()->blockEvents(!viewportFocused && !viewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
	uint64_t textureID = framebuffer->getColorAttachmentRendererID();
	if (textureID != 0)
		ImGui::Image(reinterpret_cast<void *>(textureID), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
			const char *path = reinterpret_cast<const char *>(payload->Data);
			std::filesystem::path scenePath = core::Application::get().getAssetDirectory() / path;
			OWL_CORE_WARN("Could not load file {}: unsupported format.", scenePath.string())
		}
		ImGui::EndDragDropTarget();
	}


	ImGui::End();
	ImGui::PopStyleVar();
}


void droneLayer::renderSettings() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Drone Settings");

	ImGui::End();
	ImGui::PopStyleVar();
}

void droneLayer::renderGauges() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Drone Gauges");

	ImGui::End();
	ImGui::PopStyleVar();
}

void droneLayer::renderMisc() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Drone Misc");

	ImGui::End();
	ImGui::PopStyleVar();
}


bool droneLayer::isConnected() const {
	return connected;
}
void droneLayer::toggleConnect() {
	connected = !connected;
}

}// namespace drone
