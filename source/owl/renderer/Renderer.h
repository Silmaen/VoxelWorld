/**
 * @file Renderer.h
 * @author Silmaen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "CameraOrtho.h"
#include "RenderCommand.h"
#include "ShaderLibrary.h"
#include "TextureLibrary.h"

/**
 * @brief Namespace for the renderer elements.
 */
namespace owl::renderer {

/**
 * @brief Class Renderer.
 */
class OWL_API Renderer {
public:
	Renderer() = default;
	Renderer(const Renderer &) = delete;
	Renderer(Renderer &&) = delete;
	Renderer &operator=(const Renderer &) = delete;
	Renderer &operator=(Renderer &&) = delete;

	/**
	 * @brief Destructor.
	 */
	~Renderer() = default;

	/**
	 * @brief Initialize the renderer.
	 */
	static void init();

	/**
	 * @brief Stops the renderer.
	 */
	static void shutdown();

	/**
	 * @brief Reset the renderer.
	 */
	static void reset();

	/**
	 * @brief Get the actual type of rendering API.
	 * @return The Rendering API.
	 */
	static RenderAPI::Type getAPI() { return RenderAPI::getAPI(); }

	/**
	 * @brief The state of the renderer.
	 */
	enum struct State {
		Created,///< Renderer just created.
		Running,///< Renderer in stable state.
		Stopped,///< Renderer stopped.
		Error,  ///< Render has an error.
	};
	static State getState() { return internalState; }

	/**
	 * @brief Event on Window size change.
	 * @param width New width.
	 * @param height New height.
	 */
	static void onWindowResized(uint32_t width, uint32_t height);

	/**
	 * @brief Begins a scene.
	 */
	static void beginScene(const CameraOrtho &camera);

	/**
	 * @brief Ends a scene.
	 */
	static void endScene();

	/**
	 * @brief Access to the shader Library.
	 * @return The shader library.
	 */
	static ShaderLibrary &getShaderLibrary() { return *shaderLibrary; }

	/**
	 * @brief Access to the texture Library.
	 * @return The texture library.
	 */
	static TextureLibrary &getTextureLibrary() { return *textureLibrary; }

private:
	/// The state of the renderer.
	static State internalState;

	/**
	 * @brief Data for the current scene.
	 */
	struct SceneData {
		/// View projection Matrix.
		glm::mat4 viewProjectionMatrix;
	};
	/// The actual sceneData.
	static shared<SceneData> sceneData;
	/// Actual library of shaders.
	static shared<ShaderLibrary> shaderLibrary;
	/// Actual library of textures.
	static shared<TextureLibrary> textureLibrary;
};

}// namespace owl::renderer
