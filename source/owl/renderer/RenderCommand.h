/**
 * @file RenderCommand.h
 * @author Silmaen
 * @date 09/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"
#include "RenderAPI.h"

namespace owl::renderer {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class RenderCommand
 */
class OWL_API RenderCommand {
public:
	RenderCommand(const RenderCommand &) = delete;
	RenderCommand(RenderCommand &&) = delete;
	RenderCommand &operator=(const RenderCommand &) = delete;
	RenderCommand &operator=(RenderCommand &&) = delete;
	/**
	 * @brief Default constructor.
	 */
	RenderCommand() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~RenderCommand() = default;

	/**
	 * @brief Initialize the renderer
	 */
	inline static void init() {
		renderAPI->init();
	}
	
	/**
	 * @brief Define the view port for this API
	 * @param x Starting X coordinate
	 * @param y Starting Y coordinate
	 * @param width Viewport's width
	 * @param height Viewport Height
	 */
	inline static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
		renderAPI->setViewport(x, y, width, height);
	}
	/**
	 * @brief Binding to the definition of background color
	 * @param color The new background color
	 */
	inline static void setClearColor(const glm::vec4 &color) {
		renderAPI->setClearColor(color);
	}
	/**
	 * @brief Binding to clear screen
	 */
	inline static void clear() {
		renderAPI->clear();
	}
	/**
	 * @brief Binding the draw of vertex array
	 * @param vertexArray VA to render
	 * @param index The index to draw
	 */
	inline static void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray, uint32_t index=0) {
		renderAPI->drawIndexed(vertexArray, index);
	}
	/**
	 * @brief Create or replace the API base on it type
	 * @param type The type of the new render API
	 */
	static void create(const RenderAPI::Type& type);
	/**
	 * @brief Get the actual API type
	 * @return API Type
	 */
	inline static RenderAPI::Type getAPI() { return renderAPI->getAPI(); }
private:
	static uniq<RenderAPI> renderAPI;
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
