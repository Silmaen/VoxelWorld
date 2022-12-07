/**
 * @file Shader.h
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

namespace owl::renderer {

/**
 * @brief Class Shader
 */
class Shader {
public:
	Shader(const Shader &) = delete;
	Shader(Shader &&) = delete;
	Shader &operator=(const Shader &) = delete;
	Shader &operator=(Shader &&) = delete;
	/**
	 * @brief Constructor.
	 * @param vertexSrc Source of the vertex shader
	 * @param fragmentSrc Source of fragment shader
	 */
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	/**
	 * @brief Destructor.
	 */
	virtual ~Shader();

	/**
	 * @brief Activate the shader on the GPU
	 */
	void bind() const;

	/**
	 * @brief Deactivate the shader on the GPU
	 */
	void unbind() const;
private:
	/// Id of the shader in the GPU
	uint32_t rendererID = 0;
};

}// namespace owl::renderer
