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
#include "component/CircleRenderer.h"
#include "component/ID.h"
#include "component/NativeScript.h"
#include "component/SpriteRenderer.h"
#include "component/Tag.h"
#include "component/Transform.h"

namespace owl::scene {

template<typename Component>
static void copyComponent(entt::registry &dst, entt::registry &src, const std::unordered_map<core::UUID, entt::entity> &enttMap) {
	auto view = src.view<Component>();
	for (auto e: view) {
		core::UUID uuid = src.get<component::ID>(e).id;
		OWL_CORE_ASSERT(enttMap.find(uuid) != enttMap.end(), "Error: Component not found in map.")
		entt::entity dstEnttID = enttMap.at(uuid);

		auto &component = src.get<Component>(e);
		dst.emplace_or_replace<Component>(dstEnttID, component);
	}
}

template<typename Component>
static void copyComponentIfExists(Entity dst, Entity src) {
	if (src.hasComponent<Component>())
		dst.addOrReplaceComponent<Component>(src.getComponent<Component>());
}


Scene::Scene() = default;

Scene::~Scene() = default;

shared<Scene> Scene::copy(shared<Scene> other) {
	shared<Scene> newScene = mk_shared<Scene>();

	newScene->viewportWidth = other->viewportWidth;
	newScene->viewportHeight = other->viewportHeight;

	auto &srcSceneRegistry = other->registry;
	auto &dstSceneRegistry = newScene->registry;
	std::unordered_map<core::UUID, entt::entity> enttMap;

	// Create entities in new scene
	auto idView = srcSceneRegistry.view<component::ID>();
	for (auto e: idView) {
		core::UUID uuid = srcSceneRegistry.get<component::ID>(e).id;
		const auto &name = srcSceneRegistry.get<component::Tag>(e).tag;
		Entity newEntity = newScene->createEntityWithUUID(uuid, name);
		enttMap[uuid] = static_cast<entt::entity>(newEntity);
	}

	// Copy components (except IDComponent and TagComponent)
	copyComponent<component::Transform>(dstSceneRegistry, srcSceneRegistry, enttMap);
	copyComponent<component::SpriteRenderer>(dstSceneRegistry, srcSceneRegistry, enttMap);
	copyComponent<component::CircleRenderer>(dstSceneRegistry, srcSceneRegistry, enttMap);
	copyComponent<component::Camera>(dstSceneRegistry, srcSceneRegistry, enttMap);
	copyComponent<component::NativeScript>(dstSceneRegistry, srcSceneRegistry, enttMap);
	//copyComponent<component::Rigidbody2D>(dstSceneRegistry, srcSceneRegistry, enttMap);
	//copyComponent<component::BoxCollider2D>(dstSceneRegistry, srcSceneRegistry, enttMap);

	return newScene;
}


Entity Scene::createEntity(const std::string &name) {
	return createEntityWithUUID(core::UUID(), name);
}

Entity Scene::createEntityWithUUID(core::UUID uuid, const std::string &name) {
	Entity entity = {registry.create(), this};
	entity.addComponent<component::Transform>();
	auto &id = entity.addComponent<component::ID>();
	id.id = uuid;
	auto &tag = entity.addComponent<component::Tag>();
	tag.tag = name.empty() ? "Entity" : name;
	return entity;
}

void Scene::destroyEntity(Entity entity) {
	registry.destroy(entity.entityHandle);
}

void Scene::onUpdateRuntime([[maybe_unused]] const core::Timestep &ts) {
	renderer::Renderer2D::resetStats();
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
	glm::mat4 cameraTransform;
	{
		auto view = registry.view<component::Transform, component::Camera>();
		for (auto entity: view) {
			auto [transform, camera] = view.get<component::Transform, component::Camera>(entity);
			if (camera.primary) {
				mainCamera = &camera.camera;
				cameraTransform = transform.getTransform();
				break;
			}
		}
	}

	if (mainCamera) {
		renderer::Renderer2D::beginScene(*mainCamera, cameraTransform);
		//draw sprites
		{
			auto group = registry.group<component::Transform>(entt::get<component::SpriteRenderer>);
			for (auto entity: group) {
				auto [transform, sprite] = group.get<component::Transform, component::SpriteRenderer>(entity);
				renderer::Renderer2D::drawSprite(transform.getTransform(),
												 sprite,
												 static_cast<int>(entity));
			}
		}
		//draw circles
		{
			auto view = registry.view<component::Transform, component::CircleRenderer>();
			for (auto entity: view) {
				auto [transform, circle] = view.get<component::Transform, component::CircleRenderer>(entity);
				renderer::Renderer2D::drawCircle({.transform = transform.getTransform(),
												  .color = circle.color,
												  .thickness = circle.thickness,
												  .fade = circle.fade,
												  .entityID = static_cast<int>(entity)});
			}
		}
	}

	renderer::Renderer2D::endScene();
}

void Scene::onUpdateEditor([[maybe_unused]] core::Timestep ts, renderer::CameraEditor &camera) {
	renderer::Renderer2D::resetStats();
	renderer::Renderer2D::beginScene(camera);
	// Draw sprites
	{
		auto group = registry.group<component::Transform>(entt::get<component::SpriteRenderer>);
		for (auto entity: group) {
			auto [transform, sprite] = group.get<component::Transform, component::SpriteRenderer>(entity);
			renderer::Renderer2D::drawSprite(transform.getTransform(),
											 sprite,
											 static_cast<int>(entity));
		}
	}
	// Draw circles
	{
		auto view = registry.view<component::Transform, component::CircleRenderer>();
		for (auto entity: view) {
			auto [transform, circle] = view.get<component::Transform, component::CircleRenderer>(entity);
			renderer::Renderer2D::drawCircle({.transform = transform.getTransform(),
											  .color = circle.color,
											  .thickness = circle.thickness,
											  .fade = circle.fade,
											  .entityID = static_cast<int>(entity)});
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

void Scene::duplicateEntity(Entity entity) {
	std::string name = entity.getName();
	Entity newEntity = createEntity(name);

	copyComponentIfExists<component::Transform>(newEntity, entity);
	copyComponentIfExists<component::SpriteRenderer>(newEntity, entity);
	copyComponentIfExists<component::CircleRenderer>(newEntity, entity);
	copyComponentIfExists<component::Camera>(newEntity, entity);
	copyComponentIfExists<component::NativeScript>(newEntity, entity);
	//copyComponentIfExists<component::Rigidbody2D>(newEntity, entity);
	//copyComponentIfExists<component::BoxCollider2D>(newEntity, entity);
}

Entity Scene::getPrimaryCamera() {
	auto view = registry.view<component::Camera>();
	for (auto entity: view) {
		const auto &camera = view.get<component::Camera>(entity);
		if (camera.primary)
			return Entity{entity, this};
	}
	return {};
}

template<typename T>
void Scene::onComponentAdded([[maybe_unused]] Entity entity, [[maybe_unused]] T &component) {
	OWL_ASSERT(false, "Unknown component")
}

template<>
OWL_API void Scene::onComponentAdded<component::ID>([[maybe_unused]] Entity entity, [[maybe_unused]] component::ID &component) {
}

template<>
OWL_API void Scene::onComponentAdded<component::Tag>([[maybe_unused]] Entity entity, [[maybe_unused]] component::Tag &component) {
}

template<>
OWL_API void Scene::onComponentAdded<component::Transform>([[maybe_unused]] Entity entity, [[maybe_unused]] component::Transform &component) {
}

template<>
OWL_API void Scene::onComponentAdded<component::Camera>([[maybe_unused]] Entity entity, component::Camera &component) {
	if (viewportWidth > 0 && viewportHeight > 0)
		component.camera.setViewportSize(viewportWidth, viewportHeight);
}

template<>
OWL_API void Scene::onComponentAdded<component::SpriteRenderer>([[maybe_unused]] Entity entity, [[maybe_unused]] component::SpriteRenderer &component) {
}

template<>
OWL_API void Scene::onComponentAdded<component::CircleRenderer>([[maybe_unused]] Entity entity, [[maybe_unused]] component::CircleRenderer &component) {
}

template<>
OWL_API void Scene::onComponentAdded<component::NativeScript>([[maybe_unused]] Entity entity, [[maybe_unused]] component::NativeScript &component) {
}

}// namespace owl::scene
