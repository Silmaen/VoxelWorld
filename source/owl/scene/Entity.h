/**
 * @file Entity.h
 * @author Silmaen
 * @date 22/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Scene.h"
#include "component/ID.h"
#include "component/Tag.h"
#include <entt/entt.hpp>

namespace owl::scene {

/**
 * @brief Class Entity.
 */
class OWL_API Entity {
public:
	Entity() = default;
	Entity(const Entity &) = default;
	Entity(Entity &&) = default;
	Entity &operator=(const Entity &) = default;
	Entity &operator=(Entity &&) = default;

	/**
	 * @brief Destructor.
	 */
	virtual ~Entity() = default;

	/**
	 * @brief Constructor.
	 * @param handle Entity handle.
	 * @param scene Parent scene.
	 */
	Entity(entt::entity handle, Scene *scene);


	OWL_DIAG_PUSH
	OWL_DIAG_DISABLE_CLANG("-Wundefined-func-template")
	/**
	 * @brief Add component to this entity.
	 * @tparam T Type of the component.
	 * @tparam Args Type of the component's constructor arguments.
	 * @param args Arguments for the component's constructor
	 * @return The entity's new component.
	 */
	template<typename T, typename... Args>
	T &addComponent(Args &&...args) {
		OWL_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!")
		T &component = scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
		scene->onComponentAdded<T>(*this, component);
		return component;
	}

	/**
	 * @brief Add or replace component to this entity.
	 * @tparam T Type of the component.
	 * @tparam Args Type of the component's constructor arguments.
	 * @param args Arguments for the component's constructor
	 * @return The entity's new or updated component.
	 */
	template<typename T, typename... Args>
	T &addOrReplaceComponent(Args &&...args) {
		T &component = scene->registry.emplace_or_replace<T>(entityHandle, std::forward<Args>(args)...);
		scene->onComponentAdded<T>(*this, component);
		return component;
	}
	OWL_DIAG_POP

	/**
	 * @brief Access to the component of the given type.
	 * @tparam T The type of component.
	 * @return The entity's component.
	 */
	template<typename T>
	[[nodiscard]] T &getComponent() const {
		OWL_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!")
		return scene->registry.get<T>(entityHandle);
	}

	/**
	 * @brief Check if the entity has the component.
	 * @tparam T The type of component.
	 * @return true if the entity has the component.
	 */
	template<typename T>
	[[nodiscard]] bool hasComponent() const {
		return scene->registry.all_of<T>(entityHandle);
	}

	/**
	 * @brief Remove the component from entity.
	 * @tparam T The type of component.
	 */
	template<typename T>
	void removeComponent() {
		OWL_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!")
		scene->registry.remove<T>(entityHandle);
	}

	/**
	 * @brief Get entity as boolean.
	 * @return True if handle exists.
	 */
	explicit operator bool() const { return entityHandle != entt::null; }

	/**
	 * @brief Get entity as handle.
	 * @return The entity's handle.
	 */
	explicit operator entt::entity() const { return entityHandle; }

	/**
	 * @brief Get the entity as unsigned int.
	 * @return The entity's handle code.
	 */
	explicit operator uint32_t() const { return static_cast<uint32_t>(entityHandle); }

	/**
	 * @brief Get entity's UUID.
	 * @return Entity's UUID.
	 */
	[[nodiscard]] core::UUID getUUID() const { return getComponent<component::ID>().id; }

	/**
	 * @brief Get entity's name.
	 * @return Entity's name.
	 */
	[[nodiscard]] std::string getName() const { return getComponent<component::Tag>().tag; }

	/**
	 * @brief Comparison operator.
	 * @param other Other entity to compare
	 * @return true if entities are the same.
	 */
	bool operator==(const Entity &other) const {
		return entityHandle == other.entityHandle && scene == other.scene;
	}

	/**
	 * @brief Comparison operator.
	 * @param other Other entity to compare
	 * @return true if entities are not the same.
	 */
	bool operator!=(const Entity &other) const {
		return !(*this == other);
	}

private:
	/// Entity's handle.
	entt::entity entityHandle{entt::null};
	/// Parent scene.
	Scene *scene = nullptr;
	friend class Scene;
};

}// namespace owl::scene
