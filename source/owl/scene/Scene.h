/**
 * @file Scene.h
 * @author Silmen
 * @date 22/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <entt.hpp>

#include "core/Timestep.h"

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

	/**
	 * @brief Create entity and add it to registry
	 * @param name Entity's name
	 * @return The Entity
	 */
	Entity createEntity(const std::string& name = std::string());
	void destroyEntity(Entity entity);
	/**
	 * @brief Update actions
	 * @param ts The time step
	 */
	void onUpdate(const core::Timestep &ts);

	void onViewportResize(uint32_t width, uint32_t height);
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
