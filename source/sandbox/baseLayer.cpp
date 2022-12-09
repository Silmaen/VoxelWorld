/**
 * @file baseLayer.cpp
 * @author Silmaen
 * @date 10/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "baseLayer.h"
#include <imgui.h>
#include <fstream>
#include <streambuf>

inline std::string fileToString(const std::filesystem::path &file) {
	if (!exists(file)) {
		OWL_CORE_WARN("File '{}' does not exists", file.string());
		return "";
	}
	std::ifstream t(file);
	std::string str;
	t.seekg(0, std::ios::end);
	str.reserve(static_cast<size_t>(t.tellg()));
	t.seekg(0, std::ios::beg);
	str.assign((std::istreambuf_iterator<char>(t)),
			   std::istreambuf_iterator<char>());
	return str;
}


baseLayer::baseLayer() : Layer("baseLayer"), camera(-1.6f, 1.6f, -0.9f, 0.9f) {
	// ------ one triangle ----------
	vertexArray = owl::renderer::VertexArray::create();
	float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f};
	owl::shrd<owl::renderer::VertexBuffer> vertexBuffer = owl::renderer::VertexBuffer::create(vertices, sizeof(vertices));
	owl::renderer::BufferLayout layout = {
			{"a_Position", owl::renderer::ShaderDataType::Float3},
			{"a_Color", owl::renderer::ShaderDataType::Float4}};
	vertexBuffer->SetLayout(layout);
	vertexArray->addVertexBuffer(vertexBuffer);
	uint32_t indices[3] = {0, 1, 2};
	vertexArray->setIndexBuffer(owl::renderer::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
	// ------ one triangle ----------

	// -------- Square VA ------------
	float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			0.75f, 0.75f, 0.0f,
			-0.75f, 0.75f, 0.0f};
	squareVA = owl::renderer::VertexArray::create();
	owl::shrd<owl::renderer::VertexBuffer> squareVB = owl::renderer::VertexBuffer::create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({{"a_Position", owl::renderer::ShaderDataType::Float3}});
	squareVA->addVertexBuffer(squareVB);
	uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
	squareVA->setIndexBuffer(owl::renderer::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	// -------- Square VA ------------

	// -------- Shaders ---------
	auto shaderPath = owl::core::Application::get().getWorkingDirectory() / "res" / "shaders";
	shader = owl::renderer::Shader::create(fileToString(shaderPath / "basic.vert"), fileToString(shaderPath / "basic.frag"));
	blueShader = owl::renderer::Shader::create(fileToString(shaderPath / "blue.vert"), fileToString(shaderPath / "blue.frag"));
	// -------- Shaders ---------
}

void baseLayer::onUpdate(const owl::core::Timestep& ts){

	if (owl::input::Input::isKeyPressed(owl::input::key::Left))
		cameraPosition.x -= cameraMoveSpeed * ts.getSeconds();
	else if (owl::input::Input::isKeyPressed(owl::input::key::Right))
		cameraPosition.x += cameraMoveSpeed * ts.getSeconds();

	if (owl::input::Input::isKeyPressed(owl::input::key::Up))
		cameraPosition.y += cameraMoveSpeed * ts.getSeconds();
	else if (owl::input::Input::isKeyPressed(owl::input::key::Down))
		cameraPosition.y -= cameraMoveSpeed * ts.getSeconds();

	if (owl::input::Input::isKeyPressed(owl::input::key::Q))
		cameraRotation += cameraRotationSpeed * ts.getSeconds();
	if (owl::input::Input::isKeyPressed(owl::input::key::D))
		cameraRotation -= cameraRotationSpeed * ts.getSeconds();

	owl::renderer::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
	owl::renderer::RenderCommand::clear();
	camera.setPosition(cameraPosition);
	camera.setRotation(cameraRotation);
	owl::renderer::Renderer::beginScene(camera);
	owl::renderer::Renderer::submit(blueShader, squareVA);
	owl::renderer::Renderer::submit(shader, vertexArray);
	owl::renderer::Renderer::endScene();
}

void baseLayer::onImGuiRender(const owl::core::Timestep& ts){
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
