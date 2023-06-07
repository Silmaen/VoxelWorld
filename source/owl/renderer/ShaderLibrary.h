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

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
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
	virtual ~ShaderLibrary() = default;

	/**
	 * @brief Add the shader to the library.
	 * @param shader Shader to add.
	 */
	void add(const shared<Shader> &shader);

	/**
	 * @brief Add the shader to the library and rename it.
	 * @param name Name of the shader.
	 * @param shader The Shader to add.
	 */
	void addNRename(const std::string &name, const shared<Shader> &shader);

	/**
	 * @brief Add the shader to the library from the standard path.
	 * @param name Name of the shader.
	 */
	void addFromStandardPath(const std::string &name);

	/**
	 * @brief Load a shader from a file.
	 * @param file The file to read for shader.
	 * @return The shader.
	 *
	 * The shader's name is determined by the file base name.
	 */
	shared<Shader> load(const std::filesystem::path &file);

	/**
	 * @brief Access to the shader of the given name.
	 * @param name Shader's name.
	 * @return Shader's pointer or nullptr if not exists.
	 */
	shared<Shader> get(const std::string &name);

	/**
	 * @brief Verify if a shader exists.
	 * @param name Shader's name.
	 * @return True if the shader exists.
	 */
	bool exists(const std::string &name) const;

private:
	/// List of shaders.
	std::unordered_map<std::string, shared<Shader>> shaders;
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
