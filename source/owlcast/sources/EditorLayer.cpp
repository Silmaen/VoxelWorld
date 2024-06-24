/**
 * @file EditorLayer.cpp
 * @author Silmaen
 * @date 23/06/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "EditorLayer.h"

#include <../../owl/core/Application.h>
#include <../../owl/debug/Profiler.h>

namespace owl::raycaster {

EditorLayer::EditorLayer() = default;

EditorLayer::~EditorLayer() = default;

void EditorLayer::onAttach() {
	OWL_PROFILE_FUNCTION()
	core::Application::get().enableDocking();

	m_mapWindow = mkUniq<panel::MapWindow>();
	m_viewPort = mkUniq<panel::ViewPort>();
}

void EditorLayer::onDetach() {
	m_mapWindow.reset();
	m_viewPort.reset();
}

void EditorLayer::onUpdate(const core::Timestep &iTimeStep) {
	OWL_PROFILE_FUNCTION()

	m_viewPort->onUpdate(iTimeStep);
	m_mapWindow->onUpdate(iTimeStep);
}

void EditorLayer::onEvent(event::Event &ioEvent) {
	OWL_PROFILE_FUNCTION()

	event::EventDispatcher dispatcher(ioEvent);
	dispatcher.dispatch<event::KeyPressedEvent>(
			[this](auto &&PH1) { return onKeyPressed(std::forward<decltype(PH1)>(PH1)); });
	dispatcher.dispatch<event::MouseButtonPressedEvent>(
			[this](auto &&PH1) { return onMouseButtonPressed(std::forward<decltype(PH1)>(PH1)); });
}
bool EditorLayer::onKeyPressed(event::KeyPressedEvent &ioEvent) {
	// Shortcuts
	if (ioEvent.getRepeatCount() > 0)
		return false;

	// return non-blocking.
	return false;
}
bool EditorLayer::onMouseButtonPressed(event::MouseButtonPressedEvent &ioEvent) {
	if (ioEvent.getMouseButton() == input::mouse::ButtonLeft) {
		return false;
	}

	// return non-blocking.
	return false;
}

void EditorLayer::onImGuiRender(const core::Timestep &) {
	OWL_PROFILE_FUNCTION()
	//=============================================================
	renderMenu();
	//============================================================
	m_viewPort->onRender();
	m_mapWindow->onRender();
}

void EditorLayer::renderMenu() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit"))
				owl::core::Application::get().close();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

}// namespace owl::raycaster
