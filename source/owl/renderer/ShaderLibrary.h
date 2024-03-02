/**
 * @file ShaderLibrary.h
 * @author Silmaen
 * @date 17/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Shader.h"

namespace owl::renderer {

/**
 * @brief Class ShaderLibrary;
 */
class OWL_API ShaderLibrary final {
public:
	ShaderLibrary() = default;
	ShaderLibrary(const ShaderLibrary &) = default;
	ShaderLibrary(ShaderLibrary &&) = default;
	ShaderLibrary &operator=(const ShaderLibrary &) = default;
	ShaderLibrary &operator=(ShaderLibrary &&) = default;

	/**
	 * @brief Destructor.
	 */
	~ShaderLibrary();

	/**
	 * @brief Add the shader to the library.
	 * @param[in] iShader Shader to add.
	 */
	void add(const shared<Shader> &iShader);

	/**
	 * @brief Add the shader to the library and rename it.
	 * @param[in] iName Name of the shader.
	 * @param[in] iRenderer Name of the shader's related renderer.
	 * @param[in] iShader The Shader to add.
	 */
	void addNRename(const std::string &iName, const std::string &iRenderer, const shared<Shader> &iShader);

	/**
	 * @brief Add the shader to the library from the standard path.
	 * @param[in] iName Name of the shader.
	 * @param[in] iRenderer Name of the shader's related renderer.
	 */
	void addFromStandardPath(const std::string &iName, const std::string &iRenderer);

	/**
	 * @brief Load a shader from a file.
	 * @param[in] iName Name of the shader.
	 * @param[in] iRenderer Name of the shader's related renderer.
	 * @param[in] iFile The file to read for shader.
	 * @return The shader.
	 *
	 * The shader's name is determined by the file base name.
	 */
	shared<Shader> load(const std::string &iName, const std::string &iRenderer, const std::filesystem::path &iFile);

	/**
	 * @brief Access to the shader of the given name.
	 * @param[in] iName Shader's name.
	 * @param[in] iRenderer Name of the shader's related renderer.
	 * @return Shader's pointer or nullptr if not exists.
	 */
	shared<Shader> get(const std::string &iName, const std::string &iRenderer);

	/**
	 * @brief Verify if a shader exists.
	 * @param[in] iName Shader's name.
	 * @param[in] iRenderer Name of the shader's related renderer.
	 * @return True if the shader exists.
	 */
	bool exists(const std::string &iName, const std::string &iRenderer) const;

private:
	/// List of shaders.
	std::unordered_map<std::string, shared<Shader>> m_shaders;
};

}// namespace owl::renderer
