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
class OWL_API ShaderLibrary {
public:
	ShaderLibrary() = default;
	ShaderLibrary(const ShaderLibrary &) = default;
	ShaderLibrary(ShaderLibrary &&) = default;
	ShaderLibrary &operator=(const ShaderLibrary &) = default;
	ShaderLibrary &operator=(ShaderLibrary &&) = default;

	/**
	 * @brief Destructor.
	 */
	virtual ~ShaderLibrary();

	/**
	 * @brief Add the shader to the library.
	 * @param shader Shader to add.
	 */
	void add(const shared<Shader> &shader);

	/**
	 * @brief Add the shader to the library and rename it.
	 * @param name Name of the shader.
	 * @param renderer Name of the shader's related renderer.
	 * @param shader The Shader to add.
	 */
	void addNRename(const std::string &name, const std::string &renderer, const shared<Shader> &shader);

	/**
	 * @brief Add the shader to the library from the standard path.
	 * @param name Name of the shader.
	 * @param renderer Name of the shader's related renderer.
	 */
	void addFromStandardPath(const std::string &name, const std::string &renderer);

	/**
	 * @brief Load a shader from a file.
	 * @param name Name of the shader.
	 * @param renderer Name of the shader's related renderer.
	 * @param file The file to read for shader.
	 * @return The shader.
	 *
	 * The shader's name is determined by the file base name.
	 */
	shared<Shader> load(const std::string &name, const std::string &renderer, const std::filesystem::path &file);

	/**
	 * @brief Access to the shader of the given name.
	 * @param name Shader's name.
	 * @param renderer Name of the shader's related renderer.
	 * @return Shader's pointer or nullptr if not exists.
	 */
	shared<Shader> get(const std::string &name, const std::string &renderer);

	/**
	 * @brief Verify if a shader exists.
	 * @param name Shader's name.
	 * @param renderer Name of the shader's related renderer.
	 * @return True if the shader exists.
	 */
	bool exists(const std::string &name, const std::string &renderer) const;

private:
	/// List of shaders.
	std::unordered_map<std::string, shared<Shader>> shaders;
};

}// namespace owl::renderer
