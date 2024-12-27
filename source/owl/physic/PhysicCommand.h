/**
 * @file PhysicCommand.h
 * @author Silmaen
 * @date 12/27/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"
#include "scene/Scene.h"

namespace owl::physic {

/**
 * @brief Class PhysicCommand.
 */
class OWL_API PhysicCommand final {
public:
	/**
	 * @brief Default constructor.
	 */
	PhysicCommand();
	/**
	 * @brief Default destructor.
	 */
	~PhysicCommand() = default;
	PhysicCommand(const PhysicCommand&) = delete;
	PhysicCommand(PhysicCommand&&) = delete;
	auto operator=(const PhysicCommand&) -> PhysicCommand& = delete;
	auto operator=(PhysicCommand&&) -> PhysicCommand& = delete;

	/**
	 * @brief Initialize the physical world based on the given scene.
	 * @param iScene The Scene onto apply physics
	 */
	static void init(scene::Scene* iScene);
	/**
	 * @brief Destroy the world and unlink scene.
	 */
	static void destroy();
	/**
	 * @brief Check if physic is initiated and link to the scene.
	 * @return True if initiated.
	 */
	static auto isInitialized() -> bool;
	/**
	 * @brief Compute One physical frame.
	 * @param iTimestep The time step.
	 */
	static void frame(const core::Timestep& iTimestep);

private:
	/// Implementation class.
	class Impl;
	/// Pointer to the implementation.
	static shared<Impl> m_impl;
	/// pointer to the active scene.
	static scene::Scene* m_scene;
};

}// namespace owl::physic
