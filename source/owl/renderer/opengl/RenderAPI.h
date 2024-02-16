/**
 * @file RenderAPI.h
 * @author Silmaen
 * @date 09/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../RenderAPI.h"

/**
 * @brief Namespace for opengl specific rendering objects.
 */
namespace owl::renderer::opengl {

/**
 * @brief Class RenderAPI.
 */
class RenderAPI final: public ::owl::renderer::RenderAPI {
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
	 * @param data Draw data to render.
	 * @param indexCount Number of vertex to draw (=0 all).
	 */
	void drawData(const shared<DrawData> &data, uint32_t indexCount) override;

	/**
	 * @brief Define the line width.
	 * @param width New line width.
	 */
	void setLineWidth(float width) override;

	/**
	 * @brief Get the maximum number of texture slots.
	 * @return Number of texture slots.
	 */
	[[nodiscard]] uint32_t getMaxTextureSlots() const override;
};

}// namespace owl::renderer::opengl
