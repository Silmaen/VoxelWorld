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


#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class RenderAPI
 */
class RenderAPI {
public:
	/// Render API types
	enum struct Type {
		None = 0,  /// Null Renderer
		OpenGL = 1,/// OpenGL Renderer
		Vulkan = 2,/// Vulkan renderer API
	};
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
	virtual ~RenderAPI() = default;
	/**
	 * @brief Define the background color
	 * @param color The background color
	 */
	virtual void setClearColor(const glm::vec4& color) = 0;
	/**
	 * @brief Clear the screen
	 */
	virtual void clear() = 0;
	/**
	 * @brief Draw the vertex array
	 * @param vertexArray VA to render
	 */
	virtual void drawIndexed(const shrd<VertexArray>& vertexArray) = 0;

	/**
	 * @brief Get the actual API type
	 * @return API Type
	 */
	inline static Type getAPI() { return type; }
	/**
	 * @brief Static method to create a Render API
	 * @return Render
	 */
	static uniq<RenderAPI> create(const Type& type_);
private:
	static Type type;
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
