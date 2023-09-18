/**
 * @file Settings.cpp
 * @author Silmaen
 * @date 18/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "Settings.h"

#include <imgui.h>

namespace drone::panels {

Settings::Settings() = default;

Settings::~Settings() = default;

void Settings::onUpdate(const owl::core::Timestep &) {
}

void Settings::onRender() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Drone Settings");

	ImGui::End();
	ImGui::PopStyleVar();
}

}// namespace drone::panels
