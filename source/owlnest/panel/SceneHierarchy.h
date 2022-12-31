/**
 * @file SceneHierarchy.h
 * @author Silmaen
 * @date 26/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"
#include "core/Log.h"
#include "scene/Scene.h"
#include "scene/Entity.h"

namespace owl::panel {

/**
 * @brief Class SceneHierarchy
 */
class SceneHierarchy {
public:
	/**
	 * @brief Default copy constructor
	 */
	SceneHierarchy(const SceneHierarchy &) = default;
	/**
	 * @brief Default move constructor
	 */
	SceneHierarchy(SceneHierarchy &&) = default;
	/**
	 * @brief Default copy assignation
	 * @return this
	 */
	SceneHierarchy &operator=(const SceneHierarchy &) = default;
	/**
	 * @brief Default move assignation
	 * @return this
	 */
	SceneHierarchy &operator=(SceneHierarchy &&) = default;
	/**
	 * @brief Default constructor.
	 */
	SceneHierarchy() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~SceneHierarchy() = default;

	/**
	 * @brief Constructor
	 * @param scene The base scene
	 */
	SceneHierarchy(const shrd<scene::Scene>& scene);
	/**
	 * @brief Define the Scene context
	 * @param scene The Scene
	 */
	void setContext(const shrd<scene::Scene>& scene);
	/**
	 * @brief Action on Gui Render
	 */
	void onImGuiRender();
	/**
	 * @brief Access to the selected entity
	 * @return The selected entity
	 */
	scene::Entity getSelectedEntity()const{return selection;}
private:
	/**
	 * @brief Draw one entity node
	 * @param entity The node to draw
	 */
	void drawEntityNode(scene::Entity entity);
	/**
	 * @brief Draw the properties of a component
	 * @param entity The entity
	 */
	void drawComponents(scene::Entity entity);
	/// The scene
	shrd<scene::Scene> context;
	/// The selected item
	scene::Entity selection;
};

}// namespace panel