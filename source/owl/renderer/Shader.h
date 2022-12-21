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
#include <unordered_map>

namespace owl::renderer {

class ShaderLibrary;

enum class ShaderType {
	None,
	Vertex,
	Fragment
};

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class Shader
 */
class OWL_API Shader {
public:
	Shader(const Shader &) = default;
	Shader(Shader &&) = default;
	Shader &operator=(const Shader &) = default;
	Shader &operator=(Shader &&) = default;
	/**
	 * @brief Constructor
	 */
	Shader(const std::string &name_) : name{name_} {}
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
	 * @param shaderName Name of the files in the standard path
	 * @param vertexSrc Source of the vertex shader
	 * @param fragmentSrc Source of fragment shader
	 * @return Pointer to the shader
	 */
	static shrd<Shader> create(const std::string &shaderName, const std::string &vertexSrc, const std::string &fragmentSrc);
	/**
	 * @brief Create a new shader.
	 * @param shaderName Name of the files in the standard path
	 * @return Pointer to the shader
	 */
	static shrd<Shader> create(const std::string &shaderName);
	/**
	 * @brief Create a new shader.
	 * @param file Source of the shader
	 * @return Pointer to the shader
	 */
	static shrd<Shader> create(const std::filesystem::path &file);
	/**
	 * @brief Set shader's internal int variable
	 * @param name Shader's variable's name
	 * @param value Shader's variable's value
	 */
	virtual void setInt(const std::string &name, int value) = 0;
	/**
	 * @brief Set shader's internal int variable array
	 * @param name Shader's variable's name
	 * @param values Shader's variable's raw values
	 * @param count Amount values
	 */
	virtual void setIntArray(const std::string& name, int* values, uint32_t count) = 0;
	/**
	 * @brief Set shader's internal int variable
	 * @param name Shader's variable's name
	 * @param value Shader's variable's value
	 */
	virtual void setFloat(const std::string &name, float value) = 0;
	/**
	 * @brief Set shader's internal vector 3 variable
	 * @param name Shader's variable's name
	 * @param value Shader's variable's value
	 */
	virtual void setFloat3(const std::string &name, const glm::vec3 &value) = 0;
	/**
	 * @brief Set shader's internal vector 4 variable
	 * @param name Shader's variable's name
	 * @param value Shader's variable's value
	 */
	virtual void setFloat4(const std::string &name, const glm::vec4 &value) = 0;
	/**
	 * @brief Set shader's internal Matrix 4 variable
	 * @param name Shader's variable's name
	 * @param value Shader's variable's value
	 */
	virtual void setMat4(const std::string &name, const glm::mat4 &value) = 0;

	/**
	 * @brief get the shader's name
	 * @return Shader's name
	 */
	[[nodiscard]] virtual const std::string &getName() const { return name; }

private:
	/// Shader's name
	std::string name;
	/// Library is a friend to be able to modify name
	friend class ShaderLibrary;
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
