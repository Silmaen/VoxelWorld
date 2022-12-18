/**
 * @file base2D.cpp
 * @author Silmaen
 * @date 18/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "base2D.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>


base2D::base2D(): owl::core::layer::Layer("base2D"), cameraController{1280.0f/720.0f} {}

void base2D::onAttach() {
	auto texturePath = owl::core::Application::get().getWorkingDirectory() / "res" / "textures";
	checkerboardTexture = owl::renderer::Texture2D::create(texturePath / "CheckerBoard.png");
}

void base2D::onDetach() {}

void base2D::onUpdate(const owl::core::Timestep &ts) {
	// Update
	cameraController.onUpdate(ts);

	// Render
	owl::renderer::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	owl::renderer::RenderCommand::clear();

	owl::renderer::Renderer2D::beginScene(cameraController.getCamera());
	owl::renderer::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	owl::renderer::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	owl::renderer::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, checkerboardTexture);
	owl::renderer::Renderer2D::endScene();
}

void base2D::onEvent(owl::event::Event &event) {
	cameraController.onEvent(event);
}

void base2D::onImGuiRender(const owl::core::Timestep &ts) {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();

	auto &tracker = owl::debug::Tracker::get();
	ImGui::Begin("Statistics");
	ImGui::Text(fmt::format("FPS: {}", ts.getFps()).c_str());
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
	ImGui::End();
}
