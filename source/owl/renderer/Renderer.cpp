/**
 * @file Renderer.cpp
 * @author Silmen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Renderer.h"
#include "Renderer2D.h"


namespace owl::renderer {

Renderer::State Renderer::internalState = Renderer::State::Created;

shared<Renderer::SceneData> Renderer::sceneData = mk_shared<Renderer::SceneData>();

ShaderLibrary Renderer::shaderLibrary = ShaderLibrary();

void Renderer::init() {
	OWL_PROFILE_FUNCTION()

	RenderCommand::init();
	if (RenderCommand::getState() != RenderAPI::State::Ready) {
		internalState = State::Error;
		return;
	}

	Renderer2D::init();

	internalState = State::Running;
}

void Renderer::shutdown() {
	Renderer2D::shutdown();
	internalState = State::Stopped;
}

void Renderer::beginScene(const CameraOrtho &camera) {
	sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
}

void Renderer::endScene() {
}

void Renderer::submit(const shared<Shader> &shader, const shared<VertexArray> &object, const glm::mat4 &transform) {
	if (shader) {
		shader->bind();
		shader->setMat4("u_ViewProjection", sceneData->viewProjectionMatrix);
		shader->setMat4("u_Transform", transform);
	}
	object->bind();
	RenderCommand::drawIndexed(object);
}

void Renderer::submit(const std::string &shaderName, const shared<VertexArray> &object, const glm::mat4 &transform) {
	submit(shaderLibrary.get(shaderName), object, transform);
}

void Renderer::onWindowResized(uint32_t width, uint32_t height) {
	RenderCommand::setViewport(0, 0, width, height);
}

}// namespace owl::renderer
