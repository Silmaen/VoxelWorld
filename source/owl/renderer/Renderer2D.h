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
 * @brief Utility functions.
 */
namespace utils {

/**
 * @brief Structure holding Position Rotation Scale.
 */
struct PRS {
	/// The position.
	glm::vec3 position = glm::vec3{0.f, 0.f, 0.f};
	/// The rotation.
	float rotation = 0.f;
	/// The size.
	glm::vec2 size = glm::vec2{1.f, 1.f};
};

/**
 * @brief Convert PRS structure int transformation matrix.
 * @param transform The input PRS Structure.
 * @return The corresponding Transformation matrix.
 */
OWL_API glm::mat4 toTransform(const PRS &transform);

/**
 * @brief 2D transformation structure.
 */
struct Transform2D {
	Transform2D() = delete;

	/**
	 * @brief Constructor by transformation matrix.
	 * @param matrix Input transformation matrix.
	 */
	Transform2D(const glm::mat4 &matrix) : transform{matrix} {}

	/**
	 * @brief Constructor by PRS.
	 * @param transform_ Input PRS.
	 */
	Transform2D(const PRS &transform_) : transform{toTransform(transform_)} {}

	/// The transformation matrix.
	glm::mat4 transform;
};

}// namespace utils

/**
 * @brief Data for drawing a quad.
 */
struct OWL_API Quad2DData {
	/// Transformation of the square.
	utils::Transform2D transform;
	/// Color to apply to the quad.
	glm::vec4 color = glm::vec4{1.f, 1.f, 1.f, 1.f};
	/// Eventually the texture of the quad (plain color if nullptr).
	shared<Texture> texture = nullptr;
	/// Tilling factor of the texture.
	float tilingFactor = 1.f;
	/// unique ID for the entity.
	int entityID = -1;
};

/**
 * @brief Data for drawing a circle.
 */
struct OWL_API CircleData {
	/// Transformation of the circle.
	utils::Transform2D transform;
	/// Color to apply to the circle.
	glm::vec4 color = glm::vec4{1.f, 1.f, 1.f, 1.f};
	/// Thickness of the line.
	float thickness = 1.0f;
	/// Fading of the circle.
	float fade = 0.005f;
	/// unique ID for the entity.
	int entityID = -1;
};

/**
 * @brief Data for drawing a line.
 */
struct OWL_API LineData {
	/// Starting point.
	glm::vec3 point1;
	/// Ending point.
	glm::vec3 point2;
	/// Color to apply to the line.
	glm::vec4 color = glm::vec4{1.f, 1.f, 1.f, 1.f};
	/// unique ID for the entity.
	int entityID = -1;
};

/**
 * @brief Data for drawing a rectangle.
 */
struct OWL_API RectData {
	/// Transformation of the rectangle.
	utils::Transform2D transform;
	/// Color to apply to the rect.
	glm::vec4 color = glm::vec4{1.f, 1.f, 1.f, 1.f};
	/// unique ID for the entity.
	int entityID = -1;
};

/**
 * @brief Data for drawing a polyline.
 */
struct OWL_API PolyLineData {
	/// Transformation of the rectangle.
	utils::Transform2D transform;
	/// Set of points.
	std::vector<glm::vec3> points;
	/// If the ending point connected to starting point.
	bool closed = false;
	/// Color to apply to the rect.
	glm::vec4 color = glm::vec4{1.f, 1.f, 1.f, 1.f};
	/// unique ID for the entity.
	int entityID = -1;
};

/**
 * @brief Class Renderer2D.
 */
class OWL_API Renderer2D {
public:
	Renderer2D() = default;
	Renderer2D(const Renderer2D &) = delete;
	Renderer2D(Renderer2D &&) = delete;
	Renderer2D &operator=(const Renderer2D &) = delete;
	Renderer2D &operator=(Renderer2D &&) = delete;

	/**
	 * @brief Destructor.
	 */
	~Renderer2D() = default;

	/**
	 * @brief Initialize the renderer.
	 */
	static void init();

	/**
	 * @brief Terminate the renderer.
	 */
	static void shutdown();

	/**
	 * @brief Begins a scene.
	 */
	static void beginScene(const CameraOrtho &camera);

	/**
	 * @brief Begins a scene.
	 */
	static void beginScene(const CameraEditor &camera);

	/**
	 * @brief Begins a scene.
	 */
	static void beginScene(const Camera &camera, const glm::mat4 &transform);

	/**
	 * @brief Ends a scene.
	 */
	static void endScene();

	/**
	 * @brief Flush the vertex buffer.
	 */
	static void flush();

	// Primitives
	/**
	 * @brief Access to the actual line width.
	 * @return Actual line width.
	 */
	static float getLineWidth();

	/**
	 * @brief Define the line width.
	 * @param width New line width.
	 */
	static void setLineWidth(float width);

	/**
	 * @brief Draw a line on the screen.
	 * @param lineData The data to draw the line.
	 */
	static void drawLine(const LineData &lineData);

	/**
	 * @brief Draw a rectangle.
	 * @param rectData The data to draw the rectangle.
	 */
	static void drawRect(const RectData &rectData);

	/**
	 * @brief Draw a polyline on the screen.
	 * @param lineData The data to draw the polyline
	 */
	static void drawPolyLine(const PolyLineData &lineData);

	/**
	 * @brief Draws a circle on the screen.
	 * @param circleData Circle's properties.
	 */
	static void drawCircle(const CircleData &circleData);

	/**
	 * @brief Draws a Quad on the screen.
	 * @param quadData Quad's properties.
	 */
	static void drawQuad(const Quad2DData &quadData);

	/**
	 * @brief Draws a Debug triangle on the screen.
	 */
	static void drawDebugTriangle();
	/**
	 * @brief Draw a sprite entity on the screen.
	 * @param transform Sprite transformations.
	 * @param src The sprite to render.
	 * @param entityID The entity ID.
	 */
	static void drawSprite(const glm::mat4 &transform, const scene::component::SpriteRenderer &src, int entityID);

	/**
	 * @brief Statistics.
	 */
	struct OWL_API Statistics {
		/// Amount of draw calls.
		uint32_t drawCalls = 0;
		/// Amount of quad drawn.
		uint32_t quadCount = 0;
		/// Compute the amount of vertices.
		[[nodiscard]] uint32_t getTotalVertexCount() const { return quadCount * 4; }
		/// Compute the amount of indices.
		[[nodiscard]] uint32_t getTotalIndexCount() const { return quadCount * 6; }
	};

	/**
	 * @brief Reset the statistics data.
	 */
	static void resetStats();

	/**
	 * @brief Access to stats.
	 * @return The Stats.
	 */
	static Statistics getStats();

	/**
	 * @brief Start the next batch.
	 */
	static void nextBatch();

private:
	/**
	 * @brief Combine flush and reset.
	 */
	static void startBatch();
};

}// namespace owl::renderer
