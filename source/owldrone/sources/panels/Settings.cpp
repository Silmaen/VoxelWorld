/**
 * @file Settings.cpp
 * @author Silmaen
 * @date 18/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "Settings.h"
#include "IO/CameraSystem.h"
#include "IO/DeviceManager.h"
#include "IO/DroneSettings.h"

#include <imgui.h>

namespace drone::panels {

Settings::Settings() = default;

Settings::~Settings() = default;

void Settings::onUpdate(const owl::core::Timestep &) {
}

void Settings::onRender() {
	auto &settings = IO::DroneSettings::get();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Drone Settings");
	auto &deviceManager = IO::DeviceManager::get();
	if (ImGui::CollapsingHeader("Camera Settings")) {
		bool val = settings.useCamera;
		const auto &cameras = drone::IO::CameraSystem::get().getListOfCamera();
		size_t nbCam = cameras.size();
		if (ImGui::Checkbox("Use the camera", &val)) {
			settings.useCamera = val && (nbCam > 0);
		}
		if (val && (nbCam > 0)) {
			auto &camSys = IO::CameraSystem::get();
			auto Cam = camSys.getCurrentCamera();
			int32_t sCam = Cam.id;
			if (ImGui::BeginCombo("Camera", fmt::format("Camera {}: {}", Cam.id, Cam.name).c_str())) {
				for (const auto &camera: cameras) {
					const bool isSelected = (camera.id == Cam.id);
					if (ImGui::Selectable(fmt::format("Camera {}: {}", camera.id, camera.name).c_str(), isSelected))
						sCam = camera.id;
					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
				if (sCam != Cam.id) {
					camSys.setCamera(sCam);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Refresh List")) {
				deviceManager.updateList();
			}
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

}// namespace drone::panels
