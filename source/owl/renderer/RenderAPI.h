/**
 * @file RenderAPI.h
 * @author Silmaen
 * @date 09/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "VertexArray.h"
#include <glm/glm.hpp>

namespace owl::renderer {


/**
 * @brief Class RenderAPI
 */
class OWL_API RenderAPI {
public:
	RenderAPI() = default;
	RenderAPI(const RenderAPI &) = delete;
	RenderAPI(RenderAPI &&) = delete;
	RenderAPI &operator=(const RenderAPI &) = delete;
	RenderAPI &operator=(RenderAPI &&) = delete;

	/**
	 * @brief Destructor.
	 */
	virtual ~RenderAPI();

	/**
	 * @brief Initialize the renderer.
	 */
	virtual void init() = 0;

	/**
	 * @brief Define the view port for this API.
	 * @param x Starting X coordinate.
	 * @param y Starting Y coordinate.
	 * @param width Viewport's width.
	 * @param height Viewport Height.
	 */
	virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	/**
	 * @brief Define the background color.
	 * @param color The background color.
	 */
	virtual void setClearColor(const glm::vec4 &color) = 0;

	/**
	 * @brief Clear the screen.
	 */
	virtual void clear() = 0;

	/**
	 * @brief Binding the draw of vertex array.
	 * @param vertexArray VA to render.
	 * @param indexCount Number of vertex to draw (=0 all).
	 */
	virtual void drawIndexed(const shared<VertexArray> &vertexArray, uint32_t indexCount) = 0;

	/**
	 * @brief Draw lines.
	 * @param vertexArray VA tpo render as lines.
	 * @param vertexCount Number of vertex to draw (=0 all).
	 */
	virtual void drawLines(const shared<VertexArray> &vertexArray, uint32_t vertexCount) = 0;

	/**
	 * @brief Define the line width.
	 * @param width New line width.
	 */
	virtual void setLineWidth(float width) = 0;

	/// Render API types.
	enum struct Type {
		Null = 0,         ///< Null Renderer.
		OpenGL = 1,       ///< OpenGL 4.5 Renderer.
		OpenGL_Legacy = 2,///< Old OpenGL 2.1 Renderer.
		Vulkan = 3,       ///< Vulkan renderer API.
	};
	/// Render API states.
	enum struct State {
		Created,///< Just created.
		Ready,  ///< Ready to work.
		Error   ///< in error.
	};

	/**
	 * @brief Get the actual API type.
	 * @return API Type.
	 */
	inline static Type getAPI() { return type; }

	/**
	 * @brief Static method to create a Render API.
	 * @return Render.
	 */
	static uniq<RenderAPI> create(const Type &type_);

	/**
	 * @brief Get the actual API state.
	 * @return API State.
	 */
	inline static State getState() { return state; }

protected:
	/**
	 * @brief Define the API State.
	 * @param st The new API State.
	 */
	inline static void setState(const State &st) { state = st; }

private:
	/// The current state of the API.
	static State state;
	/// Type of Renderer API.
	static Type type;
};

}// namespace owl::renderer
