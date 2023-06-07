/**
 * @file RenderAPI.h
 * @author Silmaen
 * @date 30/06/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../RenderAPI.h"

/**
 * @brief Namespace for opengl legacy specific rendering objects
 */
namespace owl::renderer::opengl_legacy {

/**
 * @brief Class RenderAPI
 */
class RenderAPI : public ::owl::renderer::RenderAPI {
public:
	RenderAPI() = default;
	RenderAPI(const RenderAPI &) = delete;
	RenderAPI(RenderAPI &&) = delete;
	RenderAPI &operator=(const RenderAPI &) = delete;
	RenderAPI &operator=(RenderAPI &&) = delete;

	/**
	 * @brief Destructor.
	 */
	~RenderAPI() override = default;

	/**
	 * @brief Initialize the renderer.
	 */
	void init() override;

	/**
	 * @brief Define the view port for this API.
	 * @param x Starting X coordinate.
	 * @param y Starting Y coordinate.
	 * @param width Viewport's width.
	 * @param height Viewport Height.
	 */
	void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	/**
	 * @brief Define the background color.
	 * @param color The background color.
	 */
	void setClearColor(const glm::vec4 &color) override;

	/**
	 * @brief Clear the screen.
	 */
	void clear() override;

	/**
	 * @brief Binding the draw of vertex array.
	 * @param vertexArray VA to render.
	 * @param indexCount Number of vertex to draw (=0 all).
	 */
	void drawIndexed(const shared<renderer::VertexArray> &vertexArray, uint32_t indexCount) override;

	/**
	 * @brief Draw lines.
	 * @param vertexArray VA tpo render as lines.
	 * @param vertexCount Number of vertex to draw (=0 all).
	 */
	void drawLines(const shared<renderer::VertexArray> &vertexArray, uint32_t vertexCount) override;

	/**
	 * @brief Define the line width.
	 * @param width New line width.
	 */
	void setLineWidth(float width) override;
};

}// namespace owl::renderer::opengl_legacy
