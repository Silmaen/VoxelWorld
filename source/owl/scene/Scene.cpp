/**
 * @file Scene.cpp
 * @author Silmen
 * @date 22/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Scene.h"

#include "Entity.h"
#include "renderer/Renderer2D.h"

#include "component/Camera.h"
#include "component/SpriteRenderer.h"
#include "component/Tag.h"
#include "component/Transform.h"
#include "scene/component/NativeScript.h"

namespace owl::scene {


Scene::Scene() {
}

Scene::~Scene() {
}

Entity Scene::createEntity(const std::string &name) {
	Entity entity = {registry.create(), this};
	entity.addComponent<component::Transform>();
	auto &tag = entity.addComponent<component::Tag>();
	tag.tag = name.empty() ? "Entity" : name;
	return entity;
}

void Scene::onUpdate([[maybe_unused]] const core::Timestep &ts) {
	// update scripts
	{
		registry.view<component::NativeScript>().each([=, this](auto entity, auto &nsc) {
			if (!nsc.instance) {
				nsc.instance = nsc.instantiateScript();
				nsc.instance->entity = Entity{entity, this};
				nsc.instance->onCreate();
			}
			nsc.instance->onUpdate(ts);
		});
	}
	// Render 2D
	renderer::Camera *mainCamera = nullptr;
	glm::mat4 *cameraTransform = nullptr;
	{
		auto view = registry.view<component::Transform, component::Camera>();
		for (auto entity: view) {
			auto [transform, camera] = view.get<component::Transform, component::Camera>(entity);
			if (camera.primary) {
				mainCamera = &camera.camera;
				cameraTransform = &transform.transform;
				break;
			}
		}
	}

	if (mainCamera) {
		renderer::Renderer2D::beginScene(*mainCamera, *cameraTransform);
		auto group = registry.group<component::Transform>(entt::get<component::SpriteRenderer>);
		for (auto entity: group) {
			auto [transform, sprite] = group.get<component::Transform, component::SpriteRenderer>(entity);
			renderer::Renderer2D::drawQuad({.transform = transform,
											.color = sprite.color});
		}
	}

	renderer::Renderer2D::endScene();
}

void Scene::onViewportResize(uint32_t width, uint32_t height) {
	viewportWidth = width;
	viewportHeight = height;

	// Resize our non-FixedAspectRatio cameras
	auto view = registry.view<component::Camera>();
	for (auto entity: view) {
		auto &cameraComponent = view.get<component::Camera>(entity);
		if (!cameraComponent.fixedAspectRatio)
			cameraComponent.camera.setViewportSize(width, height);
	}
}

}// namespace owl::scene
