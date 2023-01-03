/**
 * @file Renderer2D.h
 * @author Silmaen
 * @date 18/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Camera.h"
#include "CameraEditor.h"
#include "CameraOrtho.h"
#include "Texture.h"
#include "scene/component/SpriteRenderer.h"

namespace owl::renderer {

/**
 * @brief Data for drawing a quad
 */
struct OWL_API Quad2DData {
	/// Position of the top-left corner
	glm::vec3 position;
	/// Size of the quad
	glm::vec2 size = glm::vec2{1.f, 1.f};
	/// Color to apply to the quad
	glm::vec4 color = glm::vec4{1.f, 1.f, 1.f, 1.f};
	/// Eventually the texture of the quad (plain color if nullptr)
	shrd<Texture> texture = nullptr;
	/// Rotation of the quad
	float rotation = 0.f;
	/// Tilling factor of the texture
	float tilingFactor = 1.f;
	int entityID;
};

/**
 * @brief Data for drawing a quad
 */
struct OWL_API Quad2DDataT {
	/// Transformation of the square
	glm::mat4 transform;
	/// Color to apply to the quad
	glm::vec4 color = glm::vec4{1.f, 1.f, 1.f, 1.f};
	/// Eventually the texture of the quad (plain color if nullptr)
	shrd<Texture> texture = nullptr;
	/// Tilling factor of the texture
	float tilingFactor = 1.f;
	int entityID;
};

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
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
	 * @brief Begins a scene
	 */
	static void beginScene(const CameraEditor &camera);
	/**
	 * @brief Begins a scene
	 */
	static void beginScene(const Camera &camera, const glm::mat4 &transform);
	/**
	 * @brief Ends a scene
	 */
	static void endScene();
	/**
	 * @brief Flush the vertex buffer
	 */
	static void flush();
	// Primitives
	/**
	 * @brief Draws a Quad on the screen
	 * @param quadData Quad's properties
	 */
	static void drawQuad(const Quad2DData &quadData);
	/**
	 * @brief Draws a Quad on the screen
	 * @param quadData Quad's properties
	 */
	static void drawQuad(const Quad2DDataT &quadData);
	/**
	 * @brief Draw a sprite entity
	 * @param transform Sprite transformations
	 * @param src The sprite to render
	 * @param entityID The entity ID
	 */
	static void drawSprite(const glm::mat4& transform, scene::component::SpriteRenderer& src, int entityID);
	/**
	 * @brief Statistics
	 */
	struct OWL_API Statistics {
		/// Amount of draw calls
		uint32_t drawCalls = 0;
		/// Amount of quad drawn
		uint32_t quadCount = 0;
		/// Compute the amount of vertices
		[[nodiscard]] uint32_t getTotalVertexCount() const { return quadCount * 4; }
		/// Compute the amount of indices
		[[nodiscard]] uint32_t getTotalIndexCount() const { return quadCount * 6; }
	};
	/**
	 * @brief Reset the statistics data
	 */
	static void resetStats();
	/**
	 * @brief Access to stats
	 * @return The Stats
	 */
	static Statistics getStats();

private:
	/**
	 * @brief Combine flush and reset
	 */
	static void startBatch();
	static void nextBatch();
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
