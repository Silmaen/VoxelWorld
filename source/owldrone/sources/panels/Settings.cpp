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
	auto &devices = deviceManager.getAllDevices();
	if (ImGui::CollapsingHeader("Camera Settings")) {
		bool val = settings.useCamera;
		size_t nbCam = drone::IO::CameraSystem::getNbCamera();
		if (ImGui::Checkbox("Use the camera", &val)) {
			settings.useCamera = val && (nbCam > 0);
		}
		if (val && (nbCam > 0)) {
			auto &camSys = IO::CameraSystem::get();
			int32_t nCam = camSys.getCurrentCamera();
			int32_t sCam = nCam;
			if (ImGui::BeginCombo("Camera", fmt::format("Camera {}", nCam).c_str())) {
				for (const auto &device: devices) {
					if (device.type != IO::Device::DeviceType::Camera) continue;
					const bool isSelected = (device.id == nCam);
					if (ImGui::Selectable(fmt::format("Camera {}: {}", device.id, device.name).c_str(), isSelected))
						sCam = device.id;
					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
				if (sCam != nCam) {
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
