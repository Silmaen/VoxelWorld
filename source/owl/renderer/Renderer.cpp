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

shrd<Renderer::SceneData> Renderer::sceneData = mk_shrd<Renderer::SceneData>();

ShaderLibrary Renderer::shaderLibrary = ShaderLibrary();

void Renderer::init() {
	OWL_PROFILE_FUNCTION()

	RenderCommand::init();
	Renderer2D::init();
}
void Renderer::shutdown() {
	Renderer2D::shutdown();
}

void Renderer::beginScene(const CameraOrtho &camera) {
	sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
}

void Renderer::endScene() {
}

void Renderer::submit(const shrd<Shader> &shader, const shrd<VertexArray> &object, const glm::mat4 &transform) {
	if (shader) {
		shader->bind();
		shader->setMat4("u_ViewProjection", sceneData->viewProjectionMatrix);
		shader->setMat4("u_Transform", transform);
	}
	object->bind();
	RenderCommand::drawIndexed(object);
}

void Renderer::submit(const std::string &shaderName, const shrd<VertexArray> &object, const glm::mat4 &transform) {
	submit(shaderLibrary.get(shaderName), object, transform);
}

void Renderer::onWindowResized(uint32_t width, uint32_t height) {
	RenderCommand::setViewport(0, 0, width, height);
}

}// namespace owl::renderer
