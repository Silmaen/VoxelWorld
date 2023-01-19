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

namespace owl {


base2D::base2D() : core::layer::Layer("base2D"), cameraController{1280.0f / 720.0f} {}

void base2D::onAttach() {
	OWL_PROFILE_FUNCTION()

	auto texturePath = core::Application::get().getAssetDirectory() / "textures";
	checkerboardTexture = renderer::Texture2D::create(texturePath / "CheckerBoard.png");
}

void base2D::onDetach() {
	OWL_PROFILE_FUNCTION()
}

void base2D::onUpdate(const core::Timestep &ts) {
	OWL_PROFILE_FUNCTION()

	// Update
	cameraController.onUpdate(ts);

	// Render
	renderer::Renderer2D::resetStats();
	{
		OWL_PROFILE_SCOPE("Render Preparation")
		renderer::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
		renderer::RenderCommand::clear();
	}
	{
		static float rotation = 0.f;
		rotation += ts.getSeconds() * 50.f;
		OWL_PROFILE_SCOPE("Render Draws")
		renderer::Renderer2D::beginScene(cameraController.getCamera());
		renderer::Renderer2D::drawQuad({
				.transform = renderer::utils::PRS{
						.position = {1.0f, 0.0f, 0.0f},
						.rotation = -45.f,
						.size = {0.8f, 0.8f}},
				.color = {0.8f, 0.2f, 0.3f, 1.0f},
		});
		renderer::Renderer2D::drawQuad({.transform = renderer::utils::PRS{.position = {-1.0f, 0.0f, 0.0f},
																		  .size = {0.8f, 0.8f}},
										.color = {0.8f, 0.2f, 0.3f, 1.0f}});
		renderer::Renderer2D::drawQuad({.transform = renderer::utils::PRS{.position = {0.5f, -0.5f, 0.0f},
																		  .size = {0.5f, 0.75f}},
										.color = squareColor});
		renderer::Renderer2D::drawQuad({.transform = renderer::utils::PRS{.position = {0.0f, 0.0f, -0.1f},
																		  .size = {20.0f, 20.0f}},
										.texture = checkerboardTexture,
										.tilingFactor = 10.f});
		renderer::Renderer2D::drawQuad({.transform = renderer::utils::PRS{.position = {-2.0f, 0.0f, 0.0f},
																		  .rotation = rotation,
																		  .size = {1.0f, 1.0f}},
										.texture = checkerboardTexture,
										.tilingFactor = 20.f});
		renderer::Renderer2D::endScene();

		renderer::Renderer2D::beginScene(cameraController.getCamera());
		int32_t id = 0;
		float scalex = 1.f;
		float scaley = 1.f;
		float marg = 0.9f;
		for (float y = -5.0f; y < 5.0f; y += scaley) {
			for (float x = -5.0f; x < 5.0f; x += scalex) {
				glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f};
				renderer::Renderer2D::drawQuad({.transform = renderer::utils::PRS{.position = {x, y, 0},
																				  .size = {scalex * marg, scaley * marg}},
												.color = color,
												.entityID = id});
				id++;
			}
		}
		renderer::Renderer2D::endScene();
	}
}

void base2D::onEvent(event::Event &event) {
	cameraController.onEvent(event);
}

void base2D::onImGuiRender(const core::Timestep &ts) {
	// ==================================================================
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
		ImGui::End();
	}
	// ==================================================================
	{
		auto &tracker = debug::Tracker::get();
		ImGui::Begin("Statistics");
		ImGui::Text("%s", fmt::format("FPS: {:.2f}", ts.getFps()).c_str());
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
	viewportSize = {core::Application::get().getWindow().getWidth(), core::Application::get().getWindow().getHeight()};
}

}// namespace owl
