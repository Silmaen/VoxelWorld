/**
 * @file Application.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Application.h"

// TEMPORARY
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <memory>

namespace owl::core {

inline std::string fileToString(const std::filesystem::path &file) {
	if (!exists(file)) {
		OWL_CORE_WARN("File '{}' does not exists", file.string());
		return "";
	}
	std::ifstream t(file);
	std::string str;
	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);
	str.assign((std::istreambuf_iterator<char>(t)),
			   std::istreambuf_iterator<char>());
	OWL_CORE_TRACE("Shader read: {}", str);
	return str;
}

Application *Application::instance = nullptr;

Application::Application() {
	OWL_CORE_ASSERT(!instance, "Application already exists!");
	instance = this;

	// Setup a working directory
	// Assuming present of a folder 'res' containing the data
	workingDirectory = absolute(std::filesystem::current_path());
	while (workingDirectory != workingDirectory.root_path()) {
		if (exists(workingDirectory / "res")) break;
		workingDirectory = workingDirectory.parent_path();
	}

	appWindow = window::Window::Create();
	appWindow->setEventCallback(
			[this](auto &&PH1) { onEvent(std::forward<decltype(PH1)>(PH1)); });
	imGuiLayer = mk_shrd<gui::ImGuiLayer>();
	pushOverlay(imGuiLayer);

	// ----------------

	vertexArray = renderer::VertexArray::create();
	float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f};
	shrd<renderer::VertexBuffer> vertexBuffer = renderer::VertexBuffer::create(vertices, sizeof(vertices));
	renderer::BufferLayout layout = {
			{"a_Position", renderer::ShaderDataType::Float3},
			{"a_Color", renderer::ShaderDataType::Float4}};
	vertexBuffer->SetLayout(layout);
	vertexArray->addVertexBuffer(vertexBuffer);

	uint32_t indices[3] = {0, 1, 2};
	shrd<renderer::IndexBuffer> indexBuffer = renderer::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));
	vertexArray->setIndexBuffer(indexBuffer);

	float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			0.75f, 0.75f, 0.0f,
			-0.75f, 0.75f, 0.0f};

	shrd<renderer::VertexBuffer> squareVB = renderer::VertexBuffer::create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({{"a_Position", renderer::ShaderDataType::Float3}});
	squareVA->addVertexBuffer(squareVB);

	uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
	shrd<renderer::IndexBuffer> squareIB = renderer::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	squareVA->setIndexBuffer(squareIB);

	auto shaderPath = workingDirectory / "res" / "shaders";
	shader = mk_uniq<renderer::Shader>(fileToString(shaderPath / "basic.vert"), fileToString(shaderPath / "basic.frag"));
	blueShader = mk_uniq<renderer::Shader>(fileToString(shaderPath / "blue.vert"), fileToString(shaderPath / "blue.frag"));
	//--------------------------
}

void Application::run() {
	while (running) {
		if (!minimized) {
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			blueShader->bind();
			squareVA->bind();
			glDrawElements(GL_TRIANGLES, squareVA->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

			shader->bind();
			vertexArray->bind();
			glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
			{
				for (auto &layer: layerStack)
					layer->onUpdate();
			}
			imGuiLayer->Begin();
			{
				for (auto &layer: layerStack)
					layer->onImGuiRender();
			}
			imGuiLayer->End();
		}
		appWindow->onUpdate();
	}
}

void Application::onEvent(event::Event &e) {

	event::EventDispatcher dispatcher(e);
	dispatcher.dispatch<event::WindowCloseEvent>([this](auto &&PH1) {
		return onWindowClosed(std::forward<decltype(PH1)>(PH1));
	});
	dispatcher.dispatch<event::WindowResizeEvent>([this](auto &&PH1) {
		return onWindowResized(std::forward<decltype(PH1)>(PH1));
	});

	for (auto &layer: layerStack) {
		if (e.handled)
			break;
		layer->onEvent(e);
	}
}

bool Application::onWindowClosed(event::WindowCloseEvent &) {
	running = false;
	return true;
}

bool Application::onWindowResized(event::WindowResizeEvent &e) {
	if (e.getWidth() == 0 || e.getHeight() == 0) {
		minimized = true;
		return false;
	}
	minimized = false;
	return false;
}

void Application::pushLayer(shrd<layer::Layer> &&layer) {
	layerStack.pushLayer(std::move(layer));
}
void Application::pushOverlay(shrd<layer::Layer> &&overlay) {
	layerStack.pushOverlay(std::move(overlay));
}

}// namespace owl::core
