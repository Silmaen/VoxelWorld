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
#include "renderer/CameraOrtho.h"

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
class OWL_API Scene final {
public:
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;
	/**
	 * @brief Default constructor.
	 */
	Scene();
	/**
	 * @brief Destructor.
	 */
	~Scene();

	/**
	 * @brief Create a copy of the scene.
	 * @param[in] iOther The scene to copy.
	 * @return Pointer to the new scene.
	 */
	static shared<Scene> copy(const shared<Scene>& iOther);

	/**
	 * @brief Create entity and add it to registry.
	 * @param[in] iName Entity's name.
	 * @return The Entity.
	 */
	Entity createEntity(const std::string& iName = std::string());

	/**
	 * @brief Create entity with UUID and add it to registry.
	 * @param[in] iName Entity's name.
	 * @param[in] iUuid The Entity's UUID.
	 * @return The Entity.
	 */
	Entity createEntityWithUUID(core::UUID iUuid, const std::string& iName = std::string());

	/**
	 * @brief Destroy n entity.
	 * @param[in,out] ioEntity Entity to destroy.
	 */
	void destroyEntity(Entity& ioEntity);

	/**
	 * @brief Update actions for the runtime.
	 * @param[in] iTimeStep The time step.
	 */
	void onUpdateRuntime(const core::Timestep& iTimeStep);

	/**
	 * @brief Update action in the editor.
	 * @param[in] iTimeStep The time step.
	 * @param[in] iCamera The editor camera.
	 */
	void onUpdateEditor(const core::Timestep& iTimeStep, const renderer::CameraEditor& iCamera);

	/**
	 * @brief Update action in the editor.
	 * @param[in] iTimeStep The time step.
	 * @param[in] iCamera The ortho camera.
	 */
	void onUpdateOrtho(const core::Timestep& iTimeStep, const renderer::CameraOrtho& iCamera);

	/**
	 * @brief Action when viewport resized.
	 * @param[in] iSize New viewport's size.
	 */
	void onViewportResize(const math::vec2ui& iSize);

	/**
	 * @brief Duplicate an entity.
	 * @param[in] iEntity Entity to duplicate.
	 * @return The created entity.
	 */
	Entity duplicateEntity(const Entity& iEntity);

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
	 * @param[in] iEntity Entity receiving new component.
	 * @param[in,out] ioComponent The new component.
	 */
	template<typename T>
	void onComponentAdded(const Entity& iEntity, T& ioComponent);

	/// Draw the elements.
	void render();
	/// The viewport's size.
	math::vec2ui m_viewportSize = {0, 0};

	friend class Entity;
	friend class ScriptableEntity;
};
}// namespace owl::scene
