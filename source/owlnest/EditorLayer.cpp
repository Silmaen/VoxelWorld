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
#include "math/Transform.h"
#include "scene/SceneSerializer.h"
#include "scene/ScriptableEntity.h"
#include "scene/component/Camera.h"
#include "scene/component/Tag.h"
#include "scene/component/Transform.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
// must be included AFTER imgui
#include <ImGuizmo.h>

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
	editorCamera = renderer::CameraEditor(30.0f, 1.778f, 0.1f, 1000.0f);
	sceneHierarchy.setContext(activeScene);
}

void EditorLayer::onDetach() {
	OWL_PROFILE_FUNCTION()
}

void EditorLayer::onUpdate(const core::Timestep &ts) {
	OWL_PROFILE_FUNCTION()


	// resize
	auto spec = framebuffer->getSpecification();
	auto width = static_cast<uint32_t>(viewportSize.x);
	auto height = static_cast<uint32_t>(viewportSize.y);
	if (width > 0 && height > 0 && (width != spec.width || height != spec.height)) {
		framebuffer->resize(width, height);
		cameraController.onResize(viewportSize.x, viewportSize.y);
		editorCamera.setViewportSize(viewportSize.x, viewportSize.y);
		activeScene->onViewportResize(width, height);
	}

	// Update
	if (viewportFocused)
		cameraController.onUpdate(ts);
	editorCamera.onUpdate(ts);

	// Render
	framebuffer->bind();
	renderer::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
	renderer::RenderCommand::clear();

	// Update scene
	activeScene->onUpdateEditor(ts, editorCamera);
	framebuffer->unbind();
}

void EditorLayer::onEvent(event::Event &event) {
	cameraController.onEvent(event);
	editorCamera.onEvent(event);

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
	core::Application::get().getImGuiLayer()->blockEvents(!viewportFocused && !viewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
	uint64_t textureID = framebuffer->getColorAttachmentRendererID();
	ImGui::Image(reinterpret_cast<void *>(textureID), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});

	renderGizmo();

	ImGui::End();
	ImGui::PopStyleVar();
}

void EditorLayer::renderGizmo() {
	// Gizmos
	scene::Entity selectedEntity = sceneHierarchy.getSelectedEntity();
	if (selectedEntity && gizmoType != -1) {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		float windowWidth = ImGui::GetWindowWidth();
		float windowHeight = ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		// Runtime camera from entity
		// auto cameraEntity = activeScene->getPrimaryCamera();
		// const auto &camera = cameraEntity.getComponent<scene::component::Camera>().camera;
		// const glm::mat4 &cameraProjection = camera.getProjection();
		// glm::mat4 cameraView = glm::inverse(cameraEntity.getComponent<scene::component::Transform>().getTransform());

		// Editor camera
		const glm::mat4& cameraProjection = editorCamera.getProjection();
		glm::mat4 cameraView = editorCamera.getViewMatrix();

		// Entity transform
		auto &tc = selectedEntity.getComponent<scene::component::Transform>();
		glm::mat4 transform = tc.getTransform();

		// Snapping
		bool snap = input::Input::isKeyPressed(input::key::LeftControl);
		float snapValue = 0.5f;// Snap to 0.5m for translation/scale
		// Snap to 45 degrees for rotation
		if (gizmoType == ImGuizmo::OPERATION::ROTATE)
			snapValue = 45.0f;

		float snapValues[3] = {snapValue, snapValue, snapValue};

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
							 (ImGuizmo::OPERATION) gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
							 nullptr, snap ? snapValues : nullptr);

		if (ImGuizmo::IsUsing()) {
			glm::vec3 translation, rotation, scale;
			math::decomposeTransform(transform, translation, rotation, scale);

			glm::vec3 deltaRotation = rotation - tc.rotation;
			tc.translation = translation;
			tc.rotation += deltaRotation;
			tc.scale = scale;
		}
	}
}

void EditorLayer::renderMenu() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New", "Ctrl+N"))
				newScene();
			ImGui::Separator();
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
				openScene();
			if (ImGui::MenuItem("Save as..", "Ctrl+Shift+S"))
				saveSceneAs();
			ImGui::Separator();
			if (ImGui::MenuItem("Exit")) owl::core::Application::get().close();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void EditorLayer::newScene() {
	activeScene = mk_shrd<scene::Scene>();
	activeScene->onViewportResize((uint32_t) viewportSize.x, (uint32_t) viewportSize.y);
	sceneHierarchy.setContext(activeScene);
}

void EditorLayer::openScene() {
	auto filepath = core::FileDialog::openFile("Owl Scene (*.owl)|owl\n");
	if (!filepath.empty()) {
		activeScene = mk_shrd<scene::Scene>();
		activeScene->onViewportResize((uint32_t) viewportSize.x, (uint32_t) viewportSize.y);
		sceneHierarchy.setContext(activeScene);
		scene::SceneSerializer serializer(activeScene);
		serializer.deserialize(filepath);
	}
}

void EditorLayer::saveSceneAs() {
	auto filepath = core::FileDialog::saveFile("Owl Scene (*.owl)|owl\n");
	if (!filepath.empty()) {
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
		// Gizmos
		case input::key::Q:
			gizmoType = -1;
			break;
		case input::key::W:
			gizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case input::key::E:
			gizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case input::key::R:
			gizmoType = ImGuizmo::OPERATION::SCALE;
			break;
	}
	return false;
}

}// namespace owl
