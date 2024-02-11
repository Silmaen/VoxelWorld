/**
 * @file Shader.h
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../Shader.h"
#include <vulkan/vulkan.h>

namespace owl::renderer::vulkan {

/**
 * @brief Class Shader.
 */
class OWL_API Shader final : public owl::renderer::Shader {
public:
	Shader(const Shader &) = delete;
	Shader(Shader &&) = delete;
	Shader &operator=(const Shader &) = delete;
	Shader &operator=(Shader &&) = delete;

	/**
	 * @brief Constructor.
	 * @param shaderName Shader's name.
	 * @param renderer Name of the shader's related renderer.
	 * @param vertexSrc Source of the vertex shader.
	 * @param fragmentSrc Source of fragment shader.
	 */
	Shader(const std::string &shaderName, const std::string &renderer, const std::string &vertexSrc, const std::string &fragmentSrc);

	/**
	 * @brief Constructor.
	 * @param shaderName Shader's name.
	 * @param renderer Name of the shader's related renderer.
	 * @param sources The shader's sources with type.
	 */
	Shader(const std::string &shaderName, const std::string &renderer, const std::unordered_map<ShaderType, std::string> &sources);

	/**
	 * @brief Constructor.
	 * @param shaderName Shader's name.
	 * @param renderer Name of the shader's related renderer.
	 * @param sources The shader source's path with type.
	 */
	Shader(const std::string &shaderName, const std::string &renderer, const std::vector<std::filesystem::path> &sources);

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
	 * @brief Compute the stages informations.
	 * @return Stage informations.
	 */
	std::vector<VkPipelineShaderStageCreateInfo> getStagesInfo();

private:
	void createShader(const std::unordered_map<ShaderType, std::string> &sources);
	void compileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string> &sources);

	std::unordered_map<ShaderType, std::vector<uint32_t>> vulkanSPIRV;
};

}// namespace owl::renderer::vulkan
