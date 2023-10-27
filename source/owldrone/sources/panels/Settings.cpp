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
	static bool camOpen = true;
	if (ImGui::CollapsingHeader("Camera Settings", camOpen)) {
		bool val = settings.useCamera;
		auto &camSys = IO::CameraSystem::get();
		const auto &cameras = camSys.getListOfCamera();
		size_t nbCam = cameras.size();
		if (ImGui::Checkbox("Use the camera", &val)) {
			settings.useCamera = val && (nbCam > 0);
		}
		if (val && (nbCam > 0)) {
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
				camSys.actualiseList();
			}
		}
	}
	static bool comOpen = true;
	if (ImGui::CollapsingHeader("Serial Port Setting", comOpen)) {
		auto &deviceManager = IO::DeviceManager::get();
		const auto &devices = deviceManager.getAllDevices();
		auto device = deviceManager.getCurrentDevice();
		if (!device)
			device = owl::mk_shared<IO::Device>();
		std::string c_port = device->port;
		if (ImGui::BeginCombo("Serial port", fmt::format("Serial {} ({})", device->getFriendlyName(), device->port).c_str())) {
			for (const auto &dev: devices) {
				const bool isSelected = (dev.port == device->port);
				if (ImGui::Selectable(fmt::format("Serial {} ({})", dev.getFriendlyName(), dev.port).c_str(), isSelected))
					c_port = dev.port;
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
			if (c_port != device->port) {
				deviceManager.setCurrentDevice(c_port);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Refresh List")) {
			drone::IO::CameraSystem::get().actualiseList();
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

}// namespace drone::panels
