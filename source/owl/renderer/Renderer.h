/**
 * @file Renderer.h
 * @author Silmen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "CameraOrtho.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace owl::renderer {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class Renderer
 */
class OWL_API Renderer {
public:
	Renderer(const Renderer &) = delete;
	Renderer(Renderer &&) = delete;
	Renderer &operator=(const Renderer &) = delete;
	Renderer &operator=(Renderer &&) = delete;
	/**
	 * @brief Default constructor.
	 */
	Renderer() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~Renderer() = default;//---UNCOVER---
	/**
	 * @brief Get the actual type of rendering API
	 * @return The Rendering API
	 */
	inline static RenderAPI::Type getAPI() { return RenderAPI::getAPI(); }
	/**
	 * @brief Begins a scene
	 */
	static void beginScene(CameraOrtho &camera);

	/**
	 * @brief Ends a scene
	 */
	static void endScene();
	/**
	 * @brief Add an object to the scene
	 * @param object Object to add
	 */
	static void submit(const shrd<Shader> &shader, const shrd<VertexArray> &object);

private:
	/**
	 * @brief Data for the current scene
	 */
	struct SceneData {
		/// View projection Matrix
		glm::mat4 viewProjectionMatrix;
	};
	/// The actual sceneData
	static shrd<SceneData> sceneData;
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
