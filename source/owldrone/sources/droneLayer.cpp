/**
 * @file DroneLayer.cpp
 * @author Silmaen
 * @date 16/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "DroneLayer.h"

#include "IO/CameraSystem.h"
#include "IO/DeviceManager.h"
#include "IO/DroneSettings.h"
#include "panels/Gauges.h"
#include "panels/Information.h"
#include "panels/Settings.h"
#include "panels/Viewport.h"

#include <imgui.h>

using namespace owl;

namespace drone {


DroneLayer::DroneLayer() : Layer("DroneLayer") {}

void DroneLayer::onAttach() {
	OWL_PROFILE_FUNCTION()
	core::Application::get().enableDocking();

	// read settings
	if (const auto file = core::Application::get().getWorkingDirectory() / "droneConfig.yml"; exists(file))
		IO::DroneSettings::get().readFromFile(file);

	// device manager
	IO::DeviceManager::get().updateList();
	IO::CameraSystem::get().actualiseList();

	// icons
	auto& textureLib = renderer::Renderer::getTextureLibrary();
	textureLib.addFromStandardPath("icons/exit");
	textureLib.addFromStandardPath("icons/gauges");
	textureLib.addFromStandardPath("icons/settings");
	textureLib.addFromStandardPath("icons/connected");
	textureLib.addFromStandardPath("icons/connect");

	// remote controller
	rc = owl::mkShared<controller::RemoteController>();

	// Panels
	settings = owl::mkShared<panels::Settings>();
	gauges = owl::mkShared<panels::Gauges>();
	information = owl::mkShared<panels::Information>();
	viewport = owl::mkShared<panels::Viewport>();

	// attach remote controller
	settings->setRemoteController(rc);
	gauges->setRemoteController(rc);
	information->setRemoteController(rc);
	viewport->setRemoteController(rc);
}

void DroneLayer::onDetach() {
	OWL_PROFILE_FUNCTION()

	IO::CameraSystem::get().invalidate();
	rc.reset();
	settings.reset();
	gauges.reset();
	information.reset();
	viewport.reset();

	const auto file = core::Application::get().getWorkingDirectory() / "droneConfig.yml";
	IO::DroneSettings::get().saveToFile(file);
}

void DroneLayer::onUpdate(const core::Timestep& iTimeStep) {
	OWL_PROFILE_FUNCTION()

	renderer::Renderer2D::resetStats();

	switch (mode) {
		case DisplayMode::Settings:
			settings->onUpdate(iTimeStep);
			break;
		case DisplayMode::Gauges:
			viewport->onUpdate(iTimeStep);
			gauges->onUpdate(iTimeStep);
			information->onUpdate(iTimeStep);
			break;
	}
}

void DroneLayer::onEvent(event::Event& ioEvent) {

	event::EventDispatcher dispatcher(ioEvent);
	dispatcher.dispatch<event::KeyPressedEvent>(
			[this](auto&& PH1) { return onKeyPressed(std::forward<decltype(PH1)>(PH1)); });
	dispatcher.dispatch<event::MouseButtonPressedEvent>(
			[this](auto&& PH1) { return onMouseButtonPressed(std::forward<decltype(PH1)>(PH1)); });
}

void DroneLayer::onImGuiRender(const core::Timestep& iTimeStep) {
	OWL_PROFILE_FUNCTION()

	// ==================================================================
	if (showStats)
		renderStats(iTimeStep);
	//=============================================================
	renderMenu();
	//============================================================
	switch (mode) {
		case DisplayMode::Settings:
			settings->onRender();
			break;
		case DisplayMode::Gauges:
			if (showFakeDrone)
				renderFakeDrone(iTimeStep);
			viewport->onRender();
			gauges->onRender();
			information->onRender();
			break;
	}
	// ===== Toolbar: last, to be on top ========================
	renderToolbar();
}

void DroneLayer::renderStats(const core::Timestep& iTimeStep) {
	ImGui::Begin("Stats");
	ImGui::Text("%s", std::format("FPS: {:.2f}", iTimeStep.getFps()).c_str());
	ImGui::Separator();
	ImGui::Text("%s", std::format("Current used memory: {}", debug::TrackerAPI::globals().allocatedMemory).c_str());
	ImGui::Text("%s", std::format("Max used memory: {}", debug::TrackerAPI::globals().memoryPeek).c_str());
	ImGui::Text("%s", std::format("Allocation calls: {}", debug::TrackerAPI::globals().allocationCalls).c_str());
	ImGui::Text("%s", std::format("Deallocation calls: {}", debug::TrackerAPI::globals().deallocationCalls).c_str());
	ImGui::Separator();
	const auto stats = renderer::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %ud", stats.drawCalls);
	ImGui::Text("Quads: %ud", stats.quadCount);
	ImGui::Text("Vertices: %ud", stats.getTotalVertexCount());
	ImGui::Text("Indices: %ud", stats.getTotalIndexCount());
	ImGui::Separator();
	ImGui::Text("%s", std::format("UAV stats").c_str());
	ImGui::Text("%s", std::format("Vel: {}", rc->getHorizontalVelocity()).c_str());
	ImGui::Text("%s", std::format("VSI: {} ", rc->getVerticalVelocity()).c_str());
	ImGui::End();
}


void DroneLayer::renderFakeDrone(const core::Timestep&) {
	ImGui::Begin("FakeDrone");
	float vel = rc->getHorizontalVelocity();
	if (ImGui::SliderFloat("Velocity", &vel, -5, 100))
		rc->setHorizontalVelocity(vel);

	float verVel = rc->getVerticalVelocity();
	if (ImGui::SliderFloat("VerticalVelocity", &verVel, -25, 25))
		rc->setVerticalVelocity(verVel);

	float alt = rc->getAltitude();
	if (ImGui::SliderFloat("Altitude", &alt, -10, 250))
		rc->setAltitude(alt);

	math::vec3 rot = rc->getRotations();
	if (ImGui::SliderFloat3("Rotations", &rot.x(), -180, 180))
		rc->setRotation(rot);

	auto motors = rc->getMotorRates();
	bool modif = false;
	int motorId = 1;
	for (auto& mot: motors) {
		modif |= ImGui::SliderFloat(std::format("motor {}", motorId).c_str(), &mot, -10, 9000);
		motorId++;
	}
	if (modif)
		rc->setMotorRates(motors);


	ImGui::End();
}

void DroneLayer::renderMenu() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit"))
				owl::core::Application::get().close();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Debug")) {
			ImGui::MenuItem("Show Stats", nullptr, &showStats, true);
			ImGui::MenuItem("Show fake drone", nullptr, &showFakeDrone, true);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

//NOLINTBEGIN(readability-convert-member-functions-to-static)
auto DroneLayer::onKeyPressed(event::KeyPressedEvent& ioEvent) -> bool {
	// Shortcuts
	if (ioEvent.getRepeatCount() > 0)
		return false;

	// return non-blocking.
	return false;
}

auto DroneLayer::onMouseButtonPressed(event::MouseButtonPressedEvent& ioEvent) -> bool {
	if (ioEvent.getMouseButton() == input::mouse::ButtonLeft) {
		return false;
	}

	// return non-blocking.
	return false;
}
//NOLINTEND(readability-convert-member-functions-to-static)

OWL_DIAG_PUSH
OWL_DIAG_DISABLE_CLANG16("-Wunsafe-buffer-usage")
void DroneLayer::renderToolbar() {
	ImGui::Begin("##toolbar", nullptr,
				 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	const auto& colors = ImGui::GetStyle().Colors;
	const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
	const auto& buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));


	const float size = ImGui::GetWindowHeight() * .95f;
	const float padding = ImGui::GetWindowHeight() * .025f;
	float posX = padding;
	const auto vsize = ImVec2(size, size);

	auto& textureLib = renderer::Renderer::getTextureLibrary();

	// NOLINTBEGIN(performance-no-int-to-ptr)
	ImGui::SetCursorPos(ImVec2(posX, padding));
	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(textureLib.get("icons/settings")->getRendererId()), vsize,
						   ImVec2(0, 0), ImVec2(1, 1), 0)) {
		mode = DisplayMode::Settings;
	}
	posX += size + 2.f * padding;
	ImGui::SetCursorPos(ImVec2(posX, padding));
	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(textureLib.get("icons/gauges")->getRendererId()), vsize,
						   ImVec2(0, 1), ImVec2(1, 0), 0)) {
		mode = DisplayMode::Gauges;
	}
	const shared<renderer::Texture> iconCC =
			isConnected() ? textureLib.get("icons/connected") : textureLib.get("icons/connect");
	ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionMax().x) - (2.f * size + 3.f * padding), padding));
	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(iconCC->getRendererId()), ImVec2(size, size), ImVec2(0, 1),
						   ImVec2(1, 0), 0)) {
		toggleConnect();
	}
	ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionMax().x) - (size + padding), padding));
	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(textureLib.get("icons/exit")->getRendererId()), vsize,
						   ImVec2(0, 1), ImVec2(1, 0), 0)) {
		owl::core::Application::get().close();
	}
	// NOLINTEND(performance-no-int-to-ptr)
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
	ImGui::End();
}
OWL_DIAG_POP

auto DroneLayer::isConnected() const -> bool { return connected; }
void DroneLayer::toggleConnect() { connected = !connected; }

}// namespace drone
