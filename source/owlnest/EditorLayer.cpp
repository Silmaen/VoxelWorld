/**
 * @file EditorLayer.cpp
 * @author Silmaen
 * @date 21/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "EditorLayer.h"
#include "core/Application.h"
#include "core/utils/FileDialog.h"
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

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#include <ImGuizmo.h>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace owl {


EditorLayer::EditorLayer() : core::layer::Layer("EditorLayer"), cameraController{1280.0f / 720.0f} {}

void EditorLayer::onAttach() {
	OWL_PROFILE_FUNCTION()

	core::Application::get().enableDocking();

	renderer::FramebufferSpecification specs;
	specs.attachments = {
			renderer::FramebufferTextureFormat::RGBA8,
			renderer::FramebufferTextureFormat::RED_INTEGER,
			renderer::FramebufferTextureFormat::Depth};
	specs.width = 1280;
	specs.height = 720;
	framebuffer = renderer::Framebuffer::create(specs);

	activeScene = mk_shared<scene::Scene>();
	editorCamera = renderer::CameraEditor(30.0f, 1.778f, 0.1f, 1000.0f);

	auto iconPath = core::Application::get().getAssetDirectory() / "icons";
	iconPlay = renderer::Texture2D::create(iconPath / "PlayButton.png");
	iconStop = renderer::Texture2D::create(iconPath / "StopButton.png");
}

void EditorLayer::onDetach() {
	OWL_PROFILE_FUNCTION()

	iconPlay.reset();
	iconStop.reset();
	framebuffer.reset();
	activeScene.reset();
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

	// Render
	framebuffer->bind();
	renderer::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
	renderer::RenderCommand::clear();

	// Clear our entity ID attachment to -1
	framebuffer->clearAttachment(1, -1);

	// Update scene
	switch (state) {
		case State::Edit: {
			if (viewportFocused)
				cameraController.onUpdate(ts);
			if (viewportHovered)
				editorCamera.onUpdate(ts);
			activeScene->onUpdateEditor(ts, editorCamera);
			break;
		}
		case State::Play: {
			activeScene->onUpdateRuntime(ts);
			break;
		}
	}

	auto [mx, my] = ImGui::GetMousePos();
	mx -= viewportBounds[0].x;
	my -= viewportBounds[0].y;
	glm::vec2 viewportSize_ = viewportBounds[1] - viewportBounds[0];
	my = viewportSize_.y - my;
	int mouseX = static_cast<int>(mx);
	int mouseY = static_cast<int>(my);

	if (mouseX >= 0 && mouseY >= 0 && mouseX < static_cast<int>(viewportSize_.x) && mouseY < static_cast<int>(viewportSize_.y)) {
		int pixelData = framebuffer->readPixel(1, mouseX, mouseY);
		hoveredEntity = pixelData == -1 ? scene::Entity() : scene::Entity(static_cast<entt::entity>(pixelData), activeScene.get());
	}

	framebuffer->unbind();
}

void EditorLayer::onEvent(event::Event &event) {
	cameraController.onEvent(event);
	editorCamera.onEvent(event);

	event::EventDispatcher dispatcher(event);
	dispatcher.dispatch<event::KeyPressedEvent>([this](auto &&PH1) {
		return onKeyPressed(std::forward<decltype(PH1)>(PH1));
	});
	dispatcher.dispatch<event::MouseButtonPressedEvent>([this](auto &&PH1) {
		return onMouseButtonPressed(std::forward<decltype(PH1)>(PH1));
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
	contentBrowser.onImGuiRender();
	//=============================================================
	renderViewport();
	//=============================================================
	renderToolbar();
}

void EditorLayer::renderStats(const core::Timestep &ts) {
	auto &tracker = debug::Tracker::get();
	ImGui::Begin("Stats");
	ImGui::Text("%s", fmt::format("FPS: {:.2f}", ts.getFps()).c_str());
	ImGui::Separator();
	ImGui::Text("%s", fmt::format("Current used memory: {}",
								  tracker.globals().allocatedMemory)
							  .c_str());
	ImGui::Text("%s", fmt::format("Max used memory: {}", tracker.globals().memoryPeek)
							  .c_str());
	ImGui::Text(
			"%s", fmt::format("Allocation calls: {}", tracker.globals().allocationCalls)
						  .c_str());
	ImGui::Text("%s", fmt::format("Deallocation calls: {}",
								  tracker.globals().deallocationCalls)
							  .c_str());
	ImGui::Separator();
	std::string name = "None";
	if (hoveredEntity)
		if (hoveredEntity.hasComponent<scene::component::Tag>())
			name = hoveredEntity.getComponent<scene::component::Tag>().tag;
	ImGui::Text("Hovered Entity: %s", name.c_str());
	ImGui::Separator();
	auto stats = renderer::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());
	ImGui::Text("Viewport size: %f %f", static_cast<double>(viewportSize.x), static_cast<double>(viewportSize.y));
	ImGui::Text("Aspect ratio: %f", static_cast<double>(viewportSize.x / viewportSize.y));
	ImGui::End();
}

void EditorLayer::renderViewport() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Viewport");
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	viewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
	viewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

	viewportFocused = ImGui::IsWindowFocused();
	viewportHovered = ImGui::IsWindowHovered();
	core::Application::get().getImGuiLayer()->blockEvents(!viewportFocused && !viewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
	uint64_t textureID = framebuffer->getColorAttachmentRendererID();
	if (textureID != 0)
		ImGui::Image(reinterpret_cast<void *>(textureID), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
			const char *path = reinterpret_cast<const char *>(payload->Data);
			std::filesystem::path scenePath = core::Application::get().getAssetDirectory() / path;
			if (scenePath.extension() == ".owl") {
				openScene(scenePath);
			} else {
				OWL_CORE_WARN("Could not load {}: not a scene file", scenePath.string())
			}
		}
		ImGui::EndDragDropTarget();
	}

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

		ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y, viewportBounds[1].x - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y);

		// Runtime camera from entity
		// auto cameraEntity = activeScene->getPrimaryCamera();
		// const auto &camera = cameraEntity.getComponent<scene::component::Camera>().camera;
		// const glm::mat4 &cameraProjection = camera.getProjection();
		// glm::mat4 cameraView = glm::inverse(cameraEntity.getComponent<scene::component::Transform>().getTransform());

		// Editor camera
		const glm::mat4 &cameraProjection = editorCamera.getProjection();
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
							 static_cast<ImGuizmo::OPERATION>(gizmoType), ImGuizmo::LOCAL, glm::value_ptr(transform),
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
			if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
				openScene();
			if (ImGui::MenuItem("Save Scene", "Ctrl+S", false, !currentScenePath.empty()))
				saveCurrentScene();
			if (ImGui::MenuItem("Save Scene as..", "Ctrl+Shift+S"))
				saveSceneAs();
			ImGui::Separator();
			if (ImGui::MenuItem("Exit")) owl::core::Application::get().close();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void EditorLayer::renderToolbar() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	auto &colors = ImGui::GetStyle().Colors;
	const auto &buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
	const auto &buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

	ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	float size = ImGui::GetWindowHeight() - 4.0f;
	shared<renderer::Texture2D> icon = state == State::Edit ? iconPlay : iconStop;
	ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->getRendererID()), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0)) {
		if (state == State::Edit)
			onScenePlay();
		else if (state == State::Play)
			onSceneStop();
	}
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
	ImGui::End();
}

void EditorLayer::newScene() {
	activeScene = mk_shared<scene::Scene>();
	activeScene->onViewportResize(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
	sceneHierarchy.setContext(activeScene);
}

void EditorLayer::openScene() {
	auto filepath = core::utils::FileDialog::openFile("Owl Scene (*.owl)|owl\n");
	if (!filepath.empty()) {
		openScene(filepath);
	}
}

void EditorLayer::openScene(const std::filesystem::path &scene) {
	if (scene.extension().string() != ".owl") {
		OWL_CORE_WARN("Cannot Open file {}: not a scene", scene.string())
		return;
	}
	if (state != State::Edit)
		onSceneStop();
	auto newScene = mk_shared<scene::Scene>();
	scene::SceneSerializer serializer(newScene);
	if (serializer.deserialize(scene)) {
		editorScene = newScene;
		editorScene->onViewportResize(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
		sceneHierarchy.setContext(editorScene);
		activeScene = editorScene;
		currentScenePath = scene;
	}
}

void EditorLayer::saveSceneAs() {
	auto filepath = core::utils::FileDialog::saveFile("Owl Scene (*.owl)|owl\n");
	if (!filepath.empty()) {
		saveSceneAs(filepath);
	}
}

void EditorLayer::saveSceneAs(const std::filesystem::path &scene) {
	scene::SceneSerializer serializer(activeScene);
	serializer.serialize(scene);
	currentScenePath = scene;
}

void EditorLayer::saveCurrentScene() {
	if (currentScenePath.empty())
		saveSceneAs();
	else
		saveSceneAs(currentScenePath);
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
			if (control) {
				if (shift)
					saveSceneAs();
				else
					saveCurrentScene();
			}
			break;
		}
			// Scene Commands
		case input::key::D: {
			if (control)
				onDuplicateEntity();
			break;
		}
		// Gizmos
		case input::key::Q: {
			if (!ImGuizmo::IsUsing())
				gizmoType = -1;
			break;
		}
		case input::key::W: {
			if (!ImGuizmo::IsUsing())
				gizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case input::key::E: {
			if (!ImGuizmo::IsUsing())
				gizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case input::key::R: {
			if (!ImGuizmo::IsUsing())
				gizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
	}
	return false;
}

bool EditorLayer::onMouseButtonPressed(event::MouseButtonPressedEvent &e) {
	if (e.GetMouseButton() == input::mouse::ButtonLeft) {
		if (viewportHovered && !ImGuizmo::IsOver() && !input::Input::isKeyPressed(input::key::LeftAlt))
			sceneHierarchy.setSelectedEntity(hoveredEntity);
	}
	return false;
}

void EditorLayer::onScenePlay() {
	state = State::Play;
	activeScene = scene::Scene::copy(editorScene);

	sceneHierarchy.setContext(activeScene);
}

void EditorLayer::onSceneStop() {
	state = State::Edit;

	activeScene = editorScene;

	sceneHierarchy.setContext(activeScene);
}

void EditorLayer::onDuplicateEntity() {
	if (state != State::Edit)
		return;

	scene::Entity selectedEntity = sceneHierarchy.getSelectedEntity();
	if (selectedEntity)
		editorScene->duplicateEntity(selectedEntity);
}

}// namespace owl
