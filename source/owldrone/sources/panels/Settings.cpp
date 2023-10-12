/**
 * @file Settings.cpp
 * @author Silmaen
 * @date 18/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "Settings.h"
#include "IO/CameraSystem.h"
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
	if (ImGui::CollapsingHeader("Camera Settings")) {
		bool val = settings.useCamera;
		if (ImGui::Checkbox("Use the camera", &val)) {
			settings.useCamera = val;
		}
		if (val) {
			auto &camSys = IO::CameraSystem::get();
			int32_t nbCam = camSys.getNbCamera();
			if (nbCam > 0) {
				int32_t nCam = camSys.getCurrentCamera();
				int32_t sCam = nCam;
				if (ImGui::BeginCombo("Camera", fmt::format("Camera {}", nCam).c_str())) {
					for (int32_t iCam = 0; iCam < nbCam; ++iCam) {
						const bool is_selected = (iCam == nCam);
						if (ImGui::Selectable(fmt::format("Camera {}", iCam).c_str(), is_selected))
							sCam = iCam;
						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
					if (sCam != nCam) {
						camSys.setCamera(sCam);
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Refresh List")) {
					camSys.updateCamList();
				}
			}
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

}// namespace drone::panels
