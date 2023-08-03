/**
 * @file Scene.h
 * @author Silmaen
 * @date 22/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Timestep.h"
#include "core/UUID.h"
#include "renderer/CameraEditor.h"

#include <entt/entt.hpp>

/**
 * @brief Namespace for the Scene elements
 */
namespace owl::scene {

class Entity;
class ScriptableEntity;

/**
 * @brief Class Scene.
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

	/**
	 * @brief Create a copy of the scene.
	 * @param other The scene to copy.
	 * @return Pointer to the new scene.
	 */
	static shared<Scene> copy(const shared<Scene> &other);

	/**
	 * @brief Create entity and add it to registry.
	 * @param name Entity's name.
	 * @return The Entity.
	 */
	Entity createEntity(const std::string &name = std::string());

	/**
	 * @brief Create entity with UUID and add it to registry.
	 * @param name Entity's name.
	 * @param uuid The Entity's UUID.
	 * @return The Entity.
	 */
	Entity createEntityWithUUID(core::UUID uuid, const std::string &name = std::string());

	/**
	 * @brief Destroy n entity.
	 * @param entity Entity to destroy.
	 */
	void destroyEntity(Entity &entity);

	/**
	 * @brief Update actions for the runtime.
	 * @param ts The time step.
	 */
	void onUpdateRuntime(const core::Timestep &ts);

	/**
	 * @brief Update action in the editor.
	 * @param ts The time step.
	 * @param camera The editor camera.
	 */
	void onUpdateEditor(core::Timestep ts, renderer::CameraEditor &camera);

	/**
	 * @brief Action when viewport resized.
	 * @param width New viewport's width.
	 * @param height New viewport's height.
	 */
	void onViewportResize(uint32_t width, uint32_t height);

	/**
	 * @brief Duplicate an entity.
	 * @param entity Entity to duplicate.
	 * @return The created entity.
	 */
	Entity duplicateEntity(const Entity &entity);

	/**
	 * @brief Access to the primary Came.ra.
	 * @return The primary camera
	 */
	Entity getPrimaryCamera();

	/// Entities registry.
	entt::registry registry;

private:
	/**
	 * @brief Action when component is added to an entity.
	 * @tparam T Type of the added component.
	 * @param entity Entity receiving new component.
	 * @param component The new component.
	 */
	template<typename T>
	void onComponentAdded(const Entity &entity, T &component);

	/// The viewport's width.
	uint32_t viewportWidth = 0;

	/// The viewport's height.
	uint32_t viewportHeight = 0;

	friend class Entity;
	friend class ScriptableEntity;
};

}// namespace owl::scene
