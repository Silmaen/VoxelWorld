/**
 * @file Entity.h
 * @author Silmaen
 * @date 22/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Scene.h"
#include "entt.hpp"

namespace owl::scene {

/**
 * @brief Class Entity
 */
class OWL_API Entity {
public:
	/**
	 * @brief Default copy constructor
	 */
	Entity(const Entity &) = default;
	/**
	 * @brief Default move constructor
	 */
	Entity(Entity &&) = default;
	/**
	 * @brief Default copy assignation
	 * @return this
	 */
	Entity &operator=(const Entity &) = default;
	/**
	 * @brief Default move assignation
	 * @return this
	 */
	Entity &operator=(Entity &&) = default;
	/**
	 * @brief Default constructor.
	 */
	Entity() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~Entity() = default;

	Entity(entt::entity handle, Scene *scene);

	template<typename T, typename... Args>
	T &addComponent(Args &&...args) {
		OWL_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
		return scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	T &getComponent() {
		OWL_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
		return scene->registry.get<T>(entityHandle);
	}

	template<typename T>
	bool hasComponent() {
		return scene->registry.all_of<T>(entityHandle);
	}

	template<typename T>
	void removeComponent() {
		OWL_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
		scene->registry.remove<T>(entityHandle);
	}

	operator bool() const { return entityHandle != entt::null; }
	operator uint32_t() const {return (uint32_t)entityHandle;}

	bool operator==(const Entity& other)const {
		return entityHandle == other.entityHandle && scene == other.scene;
	}
	bool operator!=(const Entity& other)const {
		return !(*this == other);
	}

private:
	entt::entity entityHandle{entt::null};
	Scene *scene = nullptr;
};

}// namespace owl::scene
