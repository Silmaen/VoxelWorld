/**
 * @file Scene.h
 * @author Silmen
 * @date 22/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Timestep.h"
#include "renderer/CameraEditor.h"
#include "core/UUID.h"

#include <entt/entt.hpp>

/**
 * @brief Namespace for the Scene elements
 */
namespace owl::scene {

class Entity;
class ScriptableEntity;

/**
 * @brief Class Scene
 */
class OWL_API Scene {
public:
	Scene(const Scene &) = delete;
	Scene(Scene &&) = delete;
	Scene &operator=(const Scene &) = delete;
	Scene &operator=(Scene &&) = delete;
	/**
	 * @brief Default constructor.
	 */
	Scene();
	/**
	 * @brief Destructor.
	 */
	virtual ~Scene();

	static shrd<Scene> copy(shrd<Scene> other);
	/**
	 * @brief Create entity and add it to registry
	 * @param name Entity's name
	 * @return The Entity
	 */
	Entity createEntity(const std::string& name = std::string());
	/**
	 * @brief Create entity wit UUIDand add it to registry
	 * @param name Entity's name
	 * @param uuid The Entity's UUID
	 * @return The Entity
	 */
	Entity createEntityWithUUID(core::UUID uuid, const std::string& name = std::string());
	void destroyEntity(Entity entity);
	/**
	 * @brief Update actions
	 * @param ts The time step
	 */
	void onUpdateRuntime(const core::Timestep &ts);
	void onUpdateEditor(core::Timestep ts, renderer::CameraEditor& camera);

	void onViewportResize(uint32_t width, uint32_t height);

	void duplicateEntity(Entity entity);
	/**
	 * @brief Access to the primary Camera
	 * @return The primary camera
	 */
	Entity getPrimaryCamera();
	/// Entities registry
	entt::registry registry;
private:
	template<typename T>
	void onComponentAdded(Entity entity, T& component);
	uint32_t viewportWidth = 0;
	uint32_t viewportHeight = 0;

	friend class Entity;
	friend class ScriptableEntity;
};

}// namespace owl::scene
