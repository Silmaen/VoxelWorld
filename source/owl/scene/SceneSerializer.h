/**
 * @file SceneSerializer.h
 * @author Silmaen
 * @date 27/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Scene.h"

namespace owl::scene {

/**
 * @brief Class SceneSerializer.
 */
class OWL_API SceneSerializer {
public:
	/**
	 * @brief Constructor.
	 * @param scene The attached scene.
	 */
	explicit SceneSerializer(const shared<Scene> &scene);

	/**
	 * @brief Save the scene into a file.
	 * @param filepath The file where to save.
	 */
	void serialize(const std::filesystem::path &filepath);

	/**
	 * @brief Load the scene from a file.
	 * @param filepath The file to load.
	 * @return True if everything works.
	 */
	bool deserialize(const std::filesystem::path &filepath);

private:
	/// Parent Scene.
	shared<Scene> scene;
};

}// namespace owl::scene
