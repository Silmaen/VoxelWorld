/**
 * @file RenderAPI.h
 * @author Silmaen
 * @date 09/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../RenderAPI.h"

namespace owl::renderer::opengl {

/**
 * @brief Class RenderAPI
 */
class RenderAPI : public ::owl::renderer::RenderAPI {
public:
	RenderAPI(const RenderAPI &) = delete;
	RenderAPI(RenderAPI &&) = delete;
	RenderAPI &operator=(const RenderAPI &) = delete;
	RenderAPI &operator=(RenderAPI &&) = delete;
	/**
	 * @brief Default constructor.
	 */
	RenderAPI() = default;
	/**
	 * @brief Destructor.
	 */
	~RenderAPI() override = default;
	/**
	 * @brief Define the background color
	 * @param color The background color
	 */
	void setClearColor(const glm::vec4& color)override;
	/**
	 * @brief Clear the screen
	 */
	void clear()override;
	/**
	 * @brief Draw the vertex array
	 * @param vertexArray VA to render
	 */
	void drawIndexed(const shrd<VertexArray>& vertexArray)override;
};

}// namespace owl::renderer::opengl
