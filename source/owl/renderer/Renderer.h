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
#include "ShaderLibrary.h"

/**
 * @brief Namespace for the renderer elements
 */
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
	 * @brief Initialize the renderer
	 */
	static void init();
	/**
	 * @brief Stops the renderer
	 */
	static void shutdown();
	/**
	 * @brief Get the actual type of rendering API
	 * @return The Rendering API
	 */
	static RenderAPI::Type getAPI() { return RenderAPI::getAPI(); }

	/**
	 * @brief Event on Window size change
	 * @param width New width
	 * @param height New height
	 */
	static void onWindowResized(uint32_t width, uint32_t height);
	/**
	 * @brief Begins a scene
	 */
	static void beginScene(const CameraOrtho &camera);

	/**
	 * @brief Ends a scene
	 */
	static void endScene();
	/**
	 * @brief Add an object to the scene
	 * @param shader The shader to use for rendering
	 * @param object Object to add
	 * @param transform The transform to apply to object
	 */
	static void submit(const shrd<Shader> &shader, const shrd<VertexArray> &object, const glm::mat4 &transform = glm::mat4(1.0f));

	/**
	 * @brief Add an object to the scene
	 * @param shaderName The shader's name to use for rendering
	 * @param object Object to add
	 * @param transform The transform to apply to object
	 */
	static void submit(const std::string &shaderName, const shrd<VertexArray> &object, const glm::mat4 &transform = glm::mat4(1.0f));
	/**
	 * @brief Access to the shader Library
	 * @return The shader library
	 */
	static ShaderLibrary& getShaderLibrary(){return shaderLibrary;}
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
	/// Actual library of shaders
	static ShaderLibrary shaderLibrary;
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
