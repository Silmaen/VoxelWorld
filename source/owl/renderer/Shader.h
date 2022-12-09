/**
 * @file Shader.h
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/Core.h"
#include <glm/glm.hpp>

namespace owl::renderer {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class Shader
 */
class OWL_API Shader {
public:
	Shader(const Shader &) = delete;
	Shader(Shader &&) = delete;
	Shader &operator=(const Shader &) = delete;
	Shader &operator=(Shader &&) = delete;
	/**
	 * @brief Constructor
	 */
	Shader() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~Shader() = default;

	/**
	 * @brief Activate the shader on the GPU
	 */
	virtual void bind() const = 0;

	/**
	 * @brief Deactivate the shader on the GPU
	 */
	virtual void unbind() const = 0;
	/**
	 * @brief Create a new shader.
	 * @param vertexSrc Source of the vertex shader
	 * @param fragmentSrc Source of fragment shader
	 * @return pointer to the shader
	 */
	static shrd<Shader> create(const std::string &vertexSrc, const std::string &fragmentSrc);

	/**
	 * @brief Push a matrix to the GPU
	 * @param name Variable's name
	 * @param matrix The matrix data
	 */
	virtual void uploadUniformMat4(const std::string &name, const glm::mat4 &matrix) = 0;

private:
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
