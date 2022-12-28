/**
 * @file EditorLayer.cpp
 * @author Silmaen
 * @date 21/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "EditorLayer.h"
#include "core/Application.h"
#include "core/PlatformUtils.h"
#include "event/KeyEvent.h"
#include "scene/SceneSerializer.h"
#include "scene/ScriptableEntity.h"
#include "scene/component/Camera.h"
#include "scene/component/NativeScript.h"
#include "scene/component/SpriteRenderer.h"
#include "scene/component/Tag.h"
#include "scene/component/Transform.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace owl {


EditorLayer::EditorLayer() : core::layer::Layer("EditorLayer"), cameraController{1280.0f / 720.0f} {}

void EditorLayer::onAttach() {
	OWL_PROFILE_FUNCTION()

	core::Application::get().enableDocking();

	renderer::FramebufferSpecification specs;
	specs.width = 1280;
	specs.height = 720;
	framebuffer = renderer::Framebuffer::create(specs);

	activeScene = mk_shrd<scene::Scene>();

	sceneHierarchy.setContext(activeScene);
}

void EditorLayer::onDetach() {
	OWL_PROFILE_FUNCTION()
}

void EditorLayer::onUpdate(const core::Timestep &ts) {
	OWL_PROFILE_FUNCTION()

	// Update
	if (viewportFocused)
		cameraController.onUpdate(ts);

	// resize
	auto spec = framebuffer->getSpecification();
	auto width = static_cast<uint32_t>(viewportSize.x);
	auto height = static_cast<uint32_t>(viewportSize.y);
	if (width > 0 && height > 0 && (width != spec.width || height != spec.height)) {
		framebuffer->resize(width, height);
		cameraController.onResize(viewportSize.x, viewportSize.y);
		activeScene->onViewportResize(width, height);
	}

	// Render
	framebuffer->bind();
	renderer::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
	renderer::RenderCommand::clear();

	// Update scene
	activeScene->onUpdate(ts);
	framebuffer->unbind();
}

void EditorLayer::onEvent(event::Event &event) {
	cameraController.onEvent(event);

	event::EventDispatcher dispatcher(event);
	dispatcher.dispatch<event::KeyPressedEvent>([this](auto &&PH1) {
		return onKeyPressed(std::forward<decltype(PH1)>(PH1));
	});
}

void EditorLayer::onImGuiRender(const core::Timestep &ts) {
	OWL_PROFILE_FUNCTION()

	// ==================================================================
	renderStats(ts);
	//=============================================================
	renderMenu();
	//=============================================================
	sceneHierarchy.onImGuiRender();
	//=============================================================
	renderViewport();
}

void EditorLayer::renderStats(const core::Timestep &ts) {
	auto &tracker = debug::Tracker::get();
	ImGui::Begin("Stats");
	ImGui::Text(fmt::format("FPS: {:.2f}", ts.getFps()).c_str());
	ImGui::Text(fmt::format("Current used memory: {}",
							tracker.globals().allocatedMemory)
						.c_str());
	ImGui::Text(fmt::format("Max used memory: {}", tracker.globals().memoryPeek)
						.c_str());
	ImGui::Text(
			fmt::format("Allocation calls: {}", tracker.globals().allocationCalls)
					.c_str());
	ImGui::Text(fmt::format("Deallocation calls: {}",
							tracker.globals().deallocationCalls)
						.c_str());

	auto stats = renderer::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());
	ImGui::Text("Viewport size: %f %f", viewportSize.x, viewportSize.y);
	ImGui::Text("Aspect ratio: %f", viewportSize.x / viewportSize.y);
	ImGui::End();
}

void EditorLayer::renderViewport() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Viewport");

	viewportFocused = ImGui::IsWindowFocused();
	viewportHovered = ImGui::IsWindowHovered();
	core::Application::get().getImGuiLayer()->blockEvents(!viewportFocused || !viewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
	uint64_t textureID = framebuffer->getColorAttachmentRendererID();
	ImGui::Image(reinterpret_cast<void *>(textureID), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});
	ImGui::End();
	ImGui::PopStyleVar();
}

void EditorLayer::renderMenu() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New", "Ctrl+N"))
				newScene();
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
				openScene();
			if (ImGui::MenuItem("Save as..", "Ctrl+Shift+S"))
				saveSceneAs();
			if (ImGui::MenuItem("Exit")) owl::core::Application::get().close();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void EditorLayer::newScene() {
	activeScene = mk_shrd<scene::Scene>();
	activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
	sceneHierarchy.setContext(activeScene);
}

void EditorLayer::openScene() {
	auto filepath = core::FileDialog::openFile("Owl Scene (*.owl)|owl\n");
	if (!filepath.empty()){
		activeScene = mk_shrd<scene::Scene>();
		activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		sceneHierarchy.setContext(activeScene);
		scene::SceneSerializer serializer(activeScene);
		serializer.deserialize(filepath);
	}
}

void EditorLayer::saveSceneAs() {
	auto filepath = core::FileDialog::saveFile("Owl Scene (*.owl)|owl\n");
	if (!filepath.empty()){
		scene::SceneSerializer serializer(activeScene);
		serializer.serialize(filepath);
	}
}

bool EditorLayer::onKeyPressed(event::KeyPressedEvent &e) {
	// Shortcuts
	if (e.getRepeatCount() > 0)
		return false;

	bool control = input::Input::isKeyPressed(input::key::LeftControl) || input::Input::isKeyPressed(input::key::RightControl);
	bool shift = input::Input::isKeyPressed(input::key::LeftShift) || input::Input::isKeyPressed(input::key::RightShift);
	switch (e.getKeyCode()) {
		case input::key::N: {
			if (control)
				newScene();
			break;
		}
		case input::key::O: {
			if (control)
				openScene();
			break;
		}
		case input::key::S: {
			if (control && shift)
				saveSceneAs();
			break;
		}
	}
	return false;
}

}// namespace owl
