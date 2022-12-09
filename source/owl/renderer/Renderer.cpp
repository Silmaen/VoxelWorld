/**
 * @file Renderer.cpp
 * @author Silmen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Renderer.h"

namespace owl::renderer {

shrd<Renderer::SceneData> Renderer::sceneData = mk_shrd<Renderer::SceneData>();

void Renderer::beginScene(CameraOrtho& camera) {
	sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
}

void Renderer::endScene() {
}

void Renderer::submit(const shrd<Shader>& shader, const shrd<VertexArray> &object) {
	if (shader){
		shader->bind();
		shader->uploadUniformMat4("u_ViewProjection", sceneData->viewProjectionMatrix);
	}
	object->bind();
	RenderCommand::drawIndexed(object);
}
}// namespace owl::renderer
