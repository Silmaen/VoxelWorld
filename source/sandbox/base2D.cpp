/**
 * @file base2D.cpp
 * @author Silmaen
 * @date 18/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "base2D.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>


base2D::base2D() : owl::core::layer::Layer("base2D"), cameraController{1280.0f / 720.0f} {}

void base2D::onAttach() {
	OWL_PROFILE_FUNCTION()

	auto texturePath = owl::core::Application::get().getWorkingDirectory() / "res" / "textures";
	checkerboardTexture = owl::renderer::Texture2D::create(texturePath / "CheckerBoard.png");

	owl::core::Application::get().enableDocking();

	owl::renderer::FramebufferSpecification specs;
	specs.width = 1280;
	specs.height = 720;
	framebuffer = owl::renderer::Framebuffer::create(specs);
}

void base2D::onDetach() {
	OWL_PROFILE_FUNCTION()
}

void base2D::onUpdate(const owl::core::Timestep &ts) {
	OWL_PROFILE_FUNCTION()

	// Update
	cameraController.onUpdate(ts);

	// Render
	owl::renderer::Renderer2D::resetStats();
	//framebuffer->bind();
	owl::renderer::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
	owl::renderer::RenderCommand::clear();

	owl::renderer::Renderer2D::beginScene(cameraController.getCamera());
	owl::renderer::Renderer2D::drawQuad({.position = {-1.0f, 0.0f, 0.0f},
										 .size = {0.8f, 0.8f},
										 .color = {0.8f, 0.2f, 0.3f, 1.0f}});
	owl::renderer::Renderer2D::drawQuad({.position = {0.5f, -0.5f, 0.0f},
										 .size = {0.5f, 0.75f},
										 .color = squareColor});
	owl::renderer::Renderer2D::drawQuad({.position = {0.0f, 0.0f, -0.1f},
										 .size = {10.0f, 10.0f},
										 .texture = checkerboardTexture});
	owl::renderer::Renderer2D::endScene();


	owl::renderer::Renderer2D::beginScene(cameraController.getCamera());
	for (float y = -5.0f; y < 5.0f; y += 0.5f) {
		for (float x = -5.0f; x < 5.0f; x += 0.5f) {
			glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f};
			owl::renderer::Renderer2D::drawQuad({.position={x, y, 0},
												 .size={0.45f, 0.45f},
												 .color=color});
		}
	}
	owl::renderer::Renderer2D::endScene();
	//framebuffer->unbind();
}

void base2D::onEvent(owl::event::Event &event) {
	cameraController.onEvent(event);
}
#if defined (__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#endif
void base2D::onImGuiRender(const owl::core::Timestep &ts) {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	//uint32_t textureID = framebuffer->getColorAttachmentRendererID();
	uint32_t textureID = checkerboardTexture->getRendererID();
	ImGui::Image((ImTextureID)(textureID), ImVec2{ 256, 256 });
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

	auto stats = owl::renderer::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());
	ImGui::End();
}
#if defined (__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
