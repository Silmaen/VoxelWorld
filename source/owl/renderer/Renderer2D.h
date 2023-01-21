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

#include <glm/gtc/matrix_transform.hpp>

namespace owl::renderer {

namespace utils {

struct PRS {
	glm::vec3 position = glm::vec3{0.f, 0.f, 0.f};
	float rotation = 0.f;
	glm::vec2 size = glm::vec2{1.f, 1.f};
};

OWL_API glm::mat4 toTransform(const PRS& transform);

struct Transform2D {
	Transform2D() = delete;
	Transform2D(const glm::mat4 &matrix) : transform{matrix} {}
	Transform2D(const PRS& transform_) : transform{toTransform(transform_)} {}
	glm::mat4 transform;
};

}// namespace utils

/**
 * @brief Data for drawing a quad
 */
struct OWL_API Quad2DData {
	/// Transformation of the square
	utils::Transform2D transform;
	/// Color to apply to the quad
	glm::vec4 color = glm::vec4{1.f, 1.f, 1.f, 1.f};
	/// Eventually the texture of the quad (plain color if nullptr)
	shrd<Texture> texture = nullptr;
	/// Tilling factor of the texture
	float tilingFactor = 1.f;
	/// unique ID for the entity
	int entityID = -1;
};

/**
 * @brief Data for drawing a circle
 */
struct OWL_API CircleData {
	/// Transformation of the circle
	utils::Transform2D transform;
	/// Color to apply to the circle
	glm::vec4 color = glm::vec4{1.f, 1.f, 1.f, 1.f};
	/// Thickness of the line
	float thickness = 1.0f;
	/// Fading of the circle
	float fade = 0.005f;
	/// unique ID for the entity
	int entityID = -1;
};

struct OWL_API LineData {
	/// Starting point
	glm::vec3 point1;
	/// Ending point
	glm::vec3 point2;
	/// Color to apply to the line
	glm::vec4 color = glm::vec4{1.f, 1.f, 1.f, 1.f};
	/// unique ID for the entity
	int entityID = -1;
};

struct OWL_API RectData {
	/// Transformation of the rectangle
	utils::Transform2D transform;
	/// Color to apply to the rect
	glm::vec4 color = glm::vec4{1.f, 1.f, 1.f, 1.f};
	/// unique ID for the entity
	int entityID = -1;
};

struct OWL_API PolyLineData {
	/// Transformation of the rectangle
	utils::Transform2D transform;
	/// Set of points
	std::vector<glm::vec3> points;
	/// If the ending point connected to starting point
	bool closed = false;
	/// Color to apply to the rect
	glm::vec4 color = glm::vec4{1.f, 1.f, 1.f, 1.f};
	/// unique ID for the entity
	int entityID = -1;
};

#ifdef __clang__
#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wweak-vtables"
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
	~Renderer2D() = default;

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
	static float getLineWidth();
	static void setLineWidth(float width);
	static void drawLine(const LineData &lineData);
	static void drawRect(const RectData &lineData);
	static void drawPolyLine(const PolyLineData &lineData);
	/**
	 * @brief Draws a circle on the screen
	 * @param circleData Circle's properties
	 */
	static void drawCircle(const CircleData &circleData);
	/**
	 * @brief Draws a Quad on the screen
	 * @param quadData Quad's properties
	 */
	static void drawQuad(const Quad2DData &quadData);
	/**
	 * @brief Draw a sprite entity
	 * @param transform Sprite transformations
	 * @param src The sprite to render
	 * @param entityID The entity ID
	 */
	static void drawSprite(const glm::mat4 &transform, scene::component::SpriteRenderer &src, int entityID);
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
