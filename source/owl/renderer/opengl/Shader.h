/**
 * @file Shader.h
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../Shader.h"

namespace owl::renderer::opengl {

/**
 * @brief Class Shader.
 */
class OWL_API Shader : public owl::renderer::Shader {
public:
	Shader(const Shader &) = delete;
	Shader(Shader &&) = delete;
	Shader &operator=(const Shader &) = delete;
	Shader &operator=(Shader &&) = delete;

	/**
	 * @brief Constructor.
	 * @param shaderName Shader's name.
	 * @param vertexSrc Source of the vertex shader.
	 * @param fragmentSrc Source of fragment shader.
	 */
	Shader(const std::string &shaderName, const std::string &vertexSrc, const std::string &fragmentSrc);

	/**
	 * @brief Constructor.
	 * @param shaderName Shader's name.
	 * @param sources The shader's sources with type.
	 */
	Shader(const std::string &shaderName, const std::unordered_map<ShaderType, std::string> &sources);

	/**
	 * @brief Constructor.
	 * @param shaderName Shader's name.
	 * @param sources The shader source's path with type.
	 */
	Shader(const std::string &shaderName, const std::vector<std::filesystem::path> &sources);

	/**
	 * @brief Destructor.
	 */
	~Shader() override;

	/**
	 * @brief Activate the shader on the GPU.
	 */
	void bind() const override;

	/**
	 * @brief Deactivate the shader on the GPU.
	 */
	void unbind() const override;

	/**
	 * @brief Set shader's internal int variable.
	 * @param name Shader's variable's name.
	 * @param value Shader's variable's value.
	 */
	void setInt(const std::string &name, int value) override;

	/**
	 * @brief Set shader's internal int variable array.
	 * @param name Shader's variable's name.
	 * @param values Shader's variable's raw values.
	 * @param count Amount values.
	 */
	void setIntArray(const std::string &name, int *values, uint32_t count) override;

	/**
	 * @brief Set shader's internal int variable.
	 * @param name Shader's variable's name.
	 * @param value Shader's variable's value.
	 */
	void setFloat(const std::string &name, float value) override;

	/**
	 * @brief Set shader's internal vector 2 variable.
	 * @param name Shader's variable's name.
	 * @param value Shader's variable's value.
	 */
	void setFloat2(const std::string &name, const glm::vec2 &value) override;

	/**
	 * @brief Set shader's internal vector 3 variable.
	 * @param name Shader's variable's name.
	 * @param value Shader's variable's value.
	 */
	void setFloat3(const std::string &name, const glm::vec3 &value) override;

	/**
	 * @brief Set shader's internal vector 4 variable.
	 * @param name Shader's variable's name.
	 * @param value Shader's variable's value.
	 */
	void setFloat4(const std::string &name, const glm::vec4 &value) override;

	/**
	 * @brief Set shader's internal Matrix 4 variable.
	 * @param name Shader's variable's name.
	 * @param value Shader's variable's value.
	 */
	void setMat4(const std::string &name, const glm::mat4 &value) override;

	/**
	 * @brief Push a matrix to the GPU.
	 * @param name Variable's name.
	 * @param matrix The matrix data.
	 */
	void uploadUniformMat3(const std::string &name, const glm::mat3 &matrix);

	/**
	 * @brief Push a matrix to the GPU.
	 * @param name Variable's name.
	 * @param matrix The matrix data.
	 */
	void uploadUniformMat4(const std::string &name, const glm::mat4 &matrix);

	/**
	 * @brief Push an int to the GPU.
	 * @param name Variable's name.
	 * @param data The int to push.
	 */
	void uploadUniformInt(const std::string &name, int data);

	/**
	 * @brief Push an int variable array to the GPU.
	 * @param name Variable's name.
	 * @param values Variable's raw values.
	 * @param count Amount values.
	 */
	void uploadUniformIntArray(const std::string &name, int *values, uint32_t count);

	/**
	 * @brief Push a float to the GPU.
	 * @param name Variable's name.
	 * @param value The float to push.
	 */
	void uploadUniformFloat(const std::string &name, float value);

	/**
	 * @brief Push a 2-vector to the GPU.
	 * @param name Variable's name.
	 * @param value The vector to push.
	 */
	void uploadUniformFloat2(const std::string &name, const glm::vec2 &value);

	/**
	 * @brief Push a 3-vector to the GPU.
	 * @param name Variable's name.
	 * @param value The vector to push.
	 */
	void uploadUniformFloat3(const std::string &name, const glm::vec3 &value);

	/**
	 * @brief Push a 4-vector to the GPU.
	 * @param name Variable's name.
	 * @param value The vector to push.
	 */
	void uploadUniformFloat4(const std::string &name, const glm::vec4 &value);

private:
	/// Id of the shader in the GPU.
	uint32_t programID = 0;

	/**
	 * @brief Compile the given shader source code.
	 * @param sources The shader's sources with type.
	 */
	void compile(const std::unordered_map<ShaderType, std::string> &sources);

	void reflect(ShaderType stage, const std::vector<uint32_t> &shaderData);
	void compileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string> &sources);
	void compileOrGetOpenGLBinaries();
	void createProgram();

	std::unordered_map<ShaderType, std::vector<uint32_t>> vulkanSPIRV;
	std::unordered_map<ShaderType, std::vector<uint32_t>> openGLSPIRV;
	std::unordered_map<ShaderType, std::string> openGLSource;
};

}// namespace owl::renderer::opengl
