/**
 * @file GraphContext.h
 * @author Silmen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

namespace owl::renderer {

/**
 * @brief Class GraphicContext.
 */
class OWL_API GraphContext {
public:
	GraphContext() = default;
	GraphContext(const GraphContext &) = delete;
	GraphContext(GraphContext &&) = delete;
	GraphContext &operator=(const GraphContext &) = delete;
	GraphContext &operator=(GraphContext &&) = delete;

	/**
	 * @brief Destructor.
	 */
	virtual ~GraphContext();

	/**
	 * @brief Initialize the context.
	 */
	virtual void init() = 0;
	/**
	 * @brief Doo the buffer swap.
	 */
	virtual void swapBuffers() = 0;

	/**
	 * @brief Create a Graphics context.
	 * @param window The window into render context.
	 * @return The created context.
	 */
	static uniq<GraphContext> create(void *window);

private:
};

}// namespace owl::renderer
