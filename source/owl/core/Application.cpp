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

namespace owl::core {

inline std::string fileToString(const std::filesystem::path &file) {
	if (!exists(file)){
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
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	uint32_t indices[3] = {0, 1, 2};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	auto shaderPath = workingDirectory / "res" / "shaders";
	shader.reset((new renderer::Shader(fileToString(shaderPath/"basic.vert"),fileToString(shaderPath/"basic.frag"))));
	//--------------------------
}

void Application::run() {
	while (running) {
		if (!minimized) {
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			shader->bind();
			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
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
