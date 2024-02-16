/**
 * @file RenderCommand.h
 * @author Silmaen
 * @date 09/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "RenderAPI.h"
#include "core/Core.h"

namespace owl::renderer {

/**
 * @brief Class RenderCommand
 */
class OWL_API RenderCommand {
public:
	RenderCommand() = default;
	RenderCommand(const RenderCommand &) = delete;
	RenderCommand(RenderCommand &&) = delete;
	RenderCommand &operator=(const RenderCommand &) = delete;
	RenderCommand &operator=(RenderCommand &&) = delete;

	/**
	 * @brief Destructor.
	 */
	virtual ~RenderCommand();

	/**
	 * @brief Initialize the renderer.
	 */
	static void init() {
		if (renderAPI)
			renderAPI->init();
	}

	/**
	 * @brief Reset RenderAPI.
	 */
	static void invalidate() {
		renderAPI.reset();
	}

	/**
	 * @brief Get the state of the API.
	 * @return API state.
	 */
	static RenderAPI::State getState();

	/**
	 * @brief Define the view port for this API.
	 * @param x Starting X coordinate.
	 * @param y Starting Y coordinate.
	 * @param width Viewport's width.
	 * @param height Viewport Height.
	 */
	static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		renderAPI->setViewport(x, y, width, height);
	}

	/**
	 * @brief Binding to the definition of background color.
	 * @param color The new background color.
	 */
	static void setClearColor(const glm::vec4 &color) {
		renderAPI->setClearColor(color);
	}

	/**
	 * @brief Binding to clear screen.
	 */
	static void clear() {
		renderAPI->clear();
	}

	/**
	 * @brief Binding the draw of vertex array.
	 * @param data Draw data to render.
	 * @param indexCount Number of vertex to draw (=0 all).
	 */
	static void drawData(const shared<DrawData> &data, uint32_t indexCount = 0) {
		renderAPI->drawData(data, indexCount);
	}

	/**
	 * @brief Define the line width.
	 * @param width New line width.
	 */
	static void setLineWidth(float width) {
		renderAPI->setLineWidth(width);
	}

	/**
	 * @brief Create or replace the API base on it type.
	 * @param type The type of the new render API.
	 */
	static void create(const RenderAPI::Type &type);

	/**
	 * @brief Get the actual API type.
	 * @return API Type.
	 */
	static RenderAPI::Type getAPI() { return renderAPI->getAPI(); }

	/**
	 * @brief Get the maximum number of texture slots.
	 * @return Number of texture slots.
	 */
	static uint32_t getMaxTextureSlots() { return renderAPI->getMaxTextureSlots(); }

	/**
	 * @brief Reset value for the frame to render.
	 */
	static void beginFrame() { renderAPI->beginFrame(); }
	/**
	 * @brief Reset value for the batch to render.
	 */
	static void beginBatch() { renderAPI->beginBatch(); }

	/**
	 * @brief Ends draw call for the current batch.
	 */
	static void endBatch() { renderAPI->endBatch(); }
	/**
	 * @brief Ends draw call for the current frame.
	 */
	static void endFrame() { renderAPI->endFrame(); }

private:
	/// Pointer to the render API
	static uniq<RenderAPI> renderAPI;
};

}// namespace owl::renderer
