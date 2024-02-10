/**
 * @file Renderer.cpp
 * @author Silmaen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Renderer.h"
#include "Renderer2D.h"


namespace owl::renderer {

Renderer::State Renderer::internalState = Renderer::State::Created;
shared<Renderer::SceneData> Renderer::sceneData = nullptr;
shared<ShaderLibrary> Renderer::shaderLibrary = nullptr;
shared<TextureLibrary> Renderer::textureLibrary = nullptr;

void Renderer::init(bool extraDebugging) {
	OWL_PROFILE_FUNCTION()

	sceneData = mk_shared<SceneData>();
	shaderLibrary = mk_shared<ShaderLibrary>();
	textureLibrary = mk_shared<TextureLibrary>();

	RenderCommand::init(extraDebugging);
	if (RenderCommand::getState() != RenderAPI::State::Ready) {
		internalState = State::Error;
		return;
	}

	Renderer2D::init();

	internalState = State::Running;
}

void Renderer::shutdown() {
	Renderer2D::shutdown();
	reset();
	internalState = State::Stopped;
}

void Renderer::reset() {
	internalState = Renderer::State::Created;
	sceneData.reset();
	shaderLibrary.reset();
	textureLibrary.reset();
}

void Renderer::beginScene(const CameraOrtho &camera) {
	sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
}

void Renderer::endScene() {
}

void Renderer::onWindowResized(uint32_t width, uint32_t height) {
	RenderCommand::setViewport(0, 0, width, height);
}

}// namespace owl::renderer
