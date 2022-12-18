/**
 * @file baseLayer.cpp
 * @author Silmaen
 * @date 10/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "baseLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

baseLayer::baseLayer() : Layer("baseLayer"), cameraController(1280.f/720.f) {
	// -------- Square VA ------------
	{
		float squareVertices[5 * 4] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f, 0.5f, 0.0f, 0.0f, 1.0f};
		squareVA = owl::renderer::VertexArray::create();
		owl::shrd<owl::renderer::VertexBuffer> squareVB = owl::renderer::VertexBuffer::create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({{"a_Position", owl::renderer::ShaderDataType::Float3},
							 {"a_TexCoord", owl::renderer::ShaderDataType::Float2}});
		squareVA->addVertexBuffer(squareVB);
		uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
		squareVA->setIndexBuffer(owl::renderer::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	}
	// -------- Square VA ------------

	// -------- Shaders ---------
	auto &shLib = owl::renderer::Renderer::getShaderLibrary();
	shLib.addFromStandardPath("texture");
	shLib.addFromStandardPath("basic");
	auto texturePath = owl::core::Application::get().getWorkingDirectory() / "res" / "textures";
	texture = owl::renderer::Texture2D::create(texturePath / "CheckerBoard.png");
	texturePath = owl::core::Application::get().getWorkingDirectory() / "res" / "logo";
	textureLogo = owl::renderer::Texture2D::create(texturePath / "logo_owl.png");
	shLib.get("texture")->bind();
	shLib.get("texture")->setInt("u_Texture", 0);
	// -------- Shaders ---------
}


void baseLayer::onUpdate(const owl::core::Timestep &ts) {

	cameraController.onUpdate(ts);

	owl::renderer::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
	owl::renderer::RenderCommand::clear();

	owl::renderer::Renderer::beginScene(cameraController.getCamera());

	texture->bind();
	owl::renderer::Renderer::submit("texture", squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	textureLogo->bind();
	owl::renderer::Renderer::submit("texture", squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	owl::renderer::Renderer::endScene();
}

void baseLayer::onEvent(owl::event::Event &event) {
	cameraController.onEvent(event);
}

void baseLayer::onImGuiRender(const owl::core::Timestep &ts) {
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
