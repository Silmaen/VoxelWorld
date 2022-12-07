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
 * @brief Class GraphicContext
 */
class OWL_API GraphContext {
public:
	GraphContext(const GraphContext &) = delete;
	GraphContext(GraphContext &&) = delete;
	GraphContext &operator=(const GraphContext &) = delete;
	GraphContext &operator=(GraphContext &&) = delete;
	/**
	 * @brief Default constructor.
	 */
	GraphContext() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~GraphContext() = default;//---UNCOVER---

	/**
	 * @brief Initialize the context
	 */
	virtual void Init() = 0;
	/**
	 * @brief Doo the buffer swap
	 */
	virtual void SwapBuffers() = 0;

	/**
	 * @brief Create a Graphics context
	 * @param window The window into render context
	 * @return The created context
	 */
	static uniq<GraphContext> Create(void *window);

private:
};

}// namespace owl::renderer
