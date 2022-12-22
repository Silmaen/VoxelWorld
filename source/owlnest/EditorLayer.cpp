/**
 * @file EditorLayer.cpp
 * @author Silmaen
 * @date 21/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "EditorLayer.h"
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

	auto texturePath = core::Application::get().getAssetDirectory() / "textures";
	checkerboardTexture = renderer::Texture2D::create(texturePath / "CheckerBoard.png");

	core::Application::get().enableDocking();

	renderer::FramebufferSpecification specs;
	specs.width = 1280;
	specs.height = 720;
	framebuffer = renderer::Framebuffer::create(specs);

	activeScene = mk_shrd<scene::Scene>();

	// entity
	auto square = activeScene->createEntity("Green square");
	square.addComponent<scene::component::SpriteRenderer>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
	auto redSquare = activeScene->createEntity("Red Square");
	redSquare.addComponent<scene::component::SpriteRenderer>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});

	squareEntity = square;
	cameraEntity = activeScene->createEntity("Camera A");
	cameraEntity.addComponent<scene::component::Camera>();

	secondCamera = activeScene->createEntity("Camera B");
	auto &cc = secondCamera.addComponent<scene::component::Camera>();
	cc.primary = false;

	class CameraController : public scene::ScriptableEntity {
	public:
		void onCreate() override {
			auto &transform = getComponent<scene::component::Transform>().transform;
			transform[3][0] = rand() % 10 - 5.0f;
		}
		void onDestroy() override {}
		void onUpdate(core::Timestep ts) override {
			auto &transform = getComponent<scene::component::Transform>().transform;
			constexpr float speed = 5.0f;
			if (input::Input::isKeyPressed(input::key::A))
				transform[3][0] -= speed * ts.getSeconds();
			if (input::Input::isKeyPressed(input::key::D))
				transform[3][0] += speed * ts.getSeconds();
			if (input::Input::isKeyPressed(input::key::W))
				transform[3][1] -= speed * ts.getSeconds();
			if (input::Input::isKeyPressed(input::key::S))
				transform[3][1] += speed * ts.getSeconds();
		}
	};

	cameraEntity.addComponent<scene::component::NativeScript>().bind<CameraController>();
	secondCamera.addComponent<scene::component::NativeScript>().bind<CameraController>();

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
}

void EditorLayer::onImGuiRender(const core::Timestep &ts) {
	// ==================================================================
	{
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
	//=============================================================

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
			if (ImGui::MenuItem("Exit")) owl::core::Application::get().close();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	//=============================================================
	sceneHierarchy.onImGuiRender();

	//=============================================================

	renderViewport();
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


}// namespace owl
