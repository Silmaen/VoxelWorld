/**
 * @file Renderer.h
 * @author Silmen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

namespace owl::renderer {

enum struct APIType {
	None = 0,
	OpenGL=1
};

/**
 * @brief Class Renderer
 */
class Renderer {
public:
	Renderer(const Renderer &) = delete;
	Renderer(Renderer &&) = delete;
	Renderer &operator=(const Renderer &) = delete;
	Renderer &operator=(Renderer &&) = delete;
	/**
	 * @brief Default constructor.
	 */
	Renderer() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~Renderer() = default;//---UNCOVER---
	/**
	 * @brief Get the actual type of rendering API
	 * @return The Rendering API
	 */
	static APIType getAPI();
private:
};

}// namespace owl::renderer
