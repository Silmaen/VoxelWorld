/**
 * @file BasePanel.cpp
 * @author Silmaen
 * @date 10/16/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "BasePanel.h"
#include <core/Application.h>
#include <imgui.h>

namespace owl::gui {

BasePanel::BasePanel() = default;

BasePanel::~BasePanel() = default;

void BasePanel::onRender() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Viewport");
	const auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	const auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	const auto viewportOffset = ImGui::GetWindowPos();
	m_lower = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
	m_upper = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

	m_focused = ImGui::IsWindowFocused();
	m_hovered = ImGui::IsWindowHovered();
	core::Application::get().getImGuiLayer()->blockEvents(!m_focused && !m_hovered);

	const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_size = {static_cast<uint32_t>(viewportPanelSize.x), static_cast<uint32_t>(viewportPanelSize.y)};
	onRenderInternal();

	ImGui::End();
	ImGui::PopStyleVar();
}

}// namespace owl::gui