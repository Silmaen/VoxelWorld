/**
 * @file ScriptableEntity.h
 * @author Silmaen
 * @date 26/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Entity.h"

namespace owl::scene {

/**
 * @brief Class ScriptableEntity
 */
class OWL_API ScriptableEntity {
public:
	/**
	 * @brief Wrapper to access to a component
	 * @tparam T Type of component
	 * @return The component
	 */
	template<typename T>
	T& getComponent(){
		return entity.getComponent<T>();
	}
	/// The actual entity
	Entity entity;
	/**
	 * @brief Destructor
	 */
	virtual ~ScriptableEntity(){}
	/**
	 * @brief Function called on script creation
	 */
	virtual void onCreate() {}
	/**
	 * @brief Function called on script destruction
	 */
	virtual void onDestroy() {}
	/**
	 * @brief Function called on script update
	 * @param ts Timestamp
	 */
	virtual void onUpdate([[maybe_unused]]core::Timestep ts) {}
private:
	/// To access the scene privates
	friend class scene;
};

}// namespace scene
