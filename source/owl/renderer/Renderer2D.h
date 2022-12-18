/**
 * @file Renderer2D.h
 * @author Silmaen
 * @date 18/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "CameraOrtho.h"
#include "Texture.h"

namespace owl::renderer {

/**
 * @brief Class Renderer2D
 */
class OWL_API Renderer2D {
public:
	Renderer2D(const Renderer2D &) = delete;
	Renderer2D(Renderer2D &&) = delete;
	Renderer2D &operator=(const Renderer2D &) = delete;
	Renderer2D &operator=(Renderer2D &&) = delete;
	/**
	 * @brief Default constructor.
	 */
	Renderer2D() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~Renderer2D() = default;

	/**
	 * @brief Initialize the renderer
	 */
	static void init();
	/**
	 * @brief Terminate the renderer
	 */
	static void shutdown();
	/**
	 * @brief Begins a scene
	 */
	static void beginScene(const CameraOrtho &camera);
	/**
	 * @brief Ends a scene
	 */
	static void endScene();
	// Primitives
	/**
	 * @brief Draws a Quad on the screen
	 * @param position Position of the quad
	 * @param size Size of the Quad
	 * @param color Color of the quad
	 */
	static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
	/**
	 * @brief Draws a Quad on the screen
	 * @param position Position of the quad
	 * @param size Size of the Quad
	 * @param color Color of the quad
	 */
	static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
	/**
	 * @brief Draws a Quad on the screen
	 * @param position Position of the quad
	 * @param size Size of the Quad
	 * @param texture Texture of the quad
	 */
	static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const shrd<Texture> &texture);
	/**
	 * @brief Draws a Quad on the screen
	 * @param position Position of the quad
	 * @param size Size of the Quad
	 * @param texture Texture of the quad
	 */
	static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const shrd<Texture> &texture);

private:
};

}// namespace owl::renderer
