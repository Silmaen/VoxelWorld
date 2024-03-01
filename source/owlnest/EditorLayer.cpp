/**
 * @file EditorLayer.cpp
 * @author Silmaen
 * @date 21/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "EditorLayer.h"

#include <glm/gtc/type_ptr.hpp>

// must be included AFTER imgui
OWL_DIAG_PUSH
OWL_DIAG_DISABLE_CLANG("-Wzero-as-null-pointer-constant")
#include <ImGuizmo.h>
OWL_DIAG_POP

namespace owl {


EditorLayer::EditorLayer() : Layer("EditorLayer"), cameraController{1280.0f / 720.0f} {}

void EditorLayer::onAttach() {
	OWL_PROFILE_FUNCTION()

	core::Application::get().enableDocking();

	renderer::FramebufferSpecification specs;
	specs.attachments = {
			renderer::FramebufferTextureFormat::Rgba8,
			renderer::FramebufferTextureFormat::RedInteger,
			renderer::FramebufferTextureFormat::Depth};
	specs.width = 1280;
	specs.height = 720;
	framebuffer = renderer::Framebuffer::create(specs);

	activeScene = mkShared<scene::Scene>();
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

void EditorLayer::onUpdate(const core::Timestep &iTimeStep) {
	OWL_PROFILE_FUNCTION()

	// resize
	auto spec = framebuffer->getSpecification();
	const auto width = static_cast<uint32_t>(viewportSize.x);
	const auto height = static_cast<uint32_t>(viewportSize.y);
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
				cameraController.onUpdate(iTimeStep);
			if (viewportHovered)
				editorCamera.onUpdate(iTimeStep);
			activeScene->onUpdateEditor(iTimeStep, editorCamera);
			break;
		}
		case State::Play: {
			activeScene->onUpdateRuntime(iTimeStep);
			break;
		}
	}

	auto [mx, my] = ImGui::GetMousePos();
	mx -= viewportBounds[0].x;
	my -= viewportBounds[0].y;
	const glm::vec2 viewportSizeInternal = viewportBounds[1] - viewportBounds[0];
	my = viewportSizeInternal.y - my;
	const int mouseX = static_cast<int>(mx);
	const int mouseY = static_cast<int>(my);

	if (mouseX >= 0 && mouseY >= 0 && mouseX < static_cast<int>(viewportSizeInternal.x) && mouseY < static_cast<int>(
			viewportSizeInternal.y)) {
		int pixelData = framebuffer->readPixel(1, mouseX, mouseY);
		hoveredEntity = pixelData == -1
							? scene::Entity()
							: scene::Entity(static_cast<entt::entity>(pixelData), activeScene.get());
	}

	framebuffer->unbind();
}

void EditorLayer::onEvent(event::Event &ioEvent) {
	cameraController.onEvent(ioEvent);
	editorCamera.onEvent(ioEvent);

	event::EventDispatcher dispatcher(ioEvent);
	dispatcher.dispatch<event::KeyPressedEvent>([this](auto &&PH1) {
		return onKeyPressed(std::forward<decltype(PH1)>(PH1));
	});
	dispatcher.dispatch<event::MouseButtonPressedEvent>([this](auto &&PH1) {
		return onMouseButtonPressed(std::forward<decltype(PH1)>(PH1));
	});
}

void EditorLayer::onImGuiRender(const core::Timestep &iTimeStep) {
	OWL_PROFILE_FUNCTION()

	// ==================================================================
	renderStats(iTimeStep);
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

void EditorLayer::renderStats(const core::Timestep &iTimeStep) {
	auto &tracker = debug::Tracker::get();
	ImGui::Begin("Stats");
	ImGui::Text("%s", fmt::format("FPS: {:.2f}", iTimeStep.getFps()).c_str());
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
	uint64_t textureID = framebuffer->getColorAttachmentRendererId(0);
	if (textureID != 0)
		ImGui::Image(reinterpret_cast<void *>(textureID), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
			const char *path = reinterpret_cast<const char *>(payload->Data);
			std::filesystem::path scenePath = core::Application::get().getAssetDirectory() / path;
			if (scenePath.extension() == ".owl") { openScene(scenePath); } else {
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

	if (const scene::Entity selectedEntity = sceneHierarchy.getSelectedEntity(); selectedEntity && gizmoType != -1) {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y, viewportBounds[1].x - viewportBounds[0].x,
						  viewportBounds[1].y - viewportBounds[0].y);

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
		const bool snap = input::Input::isKeyPressed(input::key::LeftControl);
		float snapValue = 0.5f;// Snap to 0.5m for translation/scale
		// Snap to 45 degrees for rotation
		if (gizmoType == ImGuizmo::OPERATION::ROTATE)
			snapValue = 45.0f;

		const float snapValues[3] = {snapValue, snapValue, snapValue};

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
			if (ImGui::MenuItem("Exit"))
				owl::core::Application::get().close();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void EditorLayer::renderToolbar() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	const auto &colors = ImGui::GetStyle().Colors;
	const auto &buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
	const auto &buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

	ImGui::Begin("##toolbar", nullptr,
				 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	const float size = ImGui::GetWindowHeight() - 4.0f;
	const shared<renderer::Texture2D> icon = state == State::Edit ? iconPlay : iconStop;
	ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->getRendererId()), ImVec2(size, size), ImVec2(0, 0),
						   ImVec2(1, 1), 0)) {
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
	activeScene = mkShared<scene::Scene>();
	activeScene->onViewportResize(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
	sceneHierarchy.setContext(activeScene);
}

void EditorLayer::openScene() {
	if (const auto filepath = core::utils::FileDialog::openFile("Owl Scene (*.owl)|owl\n"); !filepath.empty()) {
		openScene(filepath);
	}
}

void EditorLayer::openScene(const std::filesystem::path &iScenePath) {
	if (iScenePath.extension().string() != ".owl") {
		OWL_CORE_WARN("Cannot Open file {}: not a scene", iScenePath.string())
		return;
	}
	if (state != State::Edit)
		onSceneStop();
	const auto newScene = mkShared<scene::Scene>();

	if (const scene::SceneSerializer serializer(newScene); serializer.deserialize(iScenePath)) {
		editorScene = newScene;
		editorScene->onViewportResize(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
		sceneHierarchy.setContext(editorScene);
		activeScene = editorScene;
		currentScenePath = iScenePath;
	}
}

void EditorLayer::saveSceneAs() {
	if (const auto filepath = core::utils::FileDialog::saveFile("Owl Scene (*.owl)|owl\n"); !filepath.empty()) {
		saveSceneAs(filepath);
	}
}

void EditorLayer::saveSceneAs(const std::filesystem::path &iScenePath) {
	const scene::SceneSerializer serializer(activeScene);
	serializer.serialize(iScenePath);
	currentScenePath = iScenePath;
}

void EditorLayer::saveCurrentScene() {
	if (currentScenePath.empty())
		saveSceneAs();
	else
		saveSceneAs(currentScenePath);
}

bool EditorLayer::onKeyPressed(const event::KeyPressedEvent &ioEvent) {
	// Shortcuts
	if (ioEvent.getRepeatCount() > 0)
		return false;

	const bool control = input::Input::isKeyPressed(input::key::LeftControl) || input::Input::isKeyPressed(
								 input::key::RightControl);
	const bool shift = input::Input::isKeyPressed(input::key::LeftShift) ||
					   input::Input::isKeyPressed(input::key::RightShift);
	switch (ioEvent.getKeyCode()) {
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

bool EditorLayer::onMouseButtonPressed(const event::MouseButtonPressedEvent &ioEvent) {
	if (ioEvent.getMouseButton() == input::mouse::ButtonLeft) {
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

void EditorLayer::onDuplicateEntity() const {
	if (state != State::Edit)
		return;

	if (const scene::Entity selectedEntity = sceneHierarchy.getSelectedEntity(); selectedEntity)
		editorScene->duplicateEntity(selectedEntity);
}

}// namespace owl
