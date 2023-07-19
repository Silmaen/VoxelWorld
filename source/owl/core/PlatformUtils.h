/**
 * @file PlatformUtils.h
 * @author Silmaen
 * @date 27/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <filesystem>

namespace owl::core {

/**
 * @brief class FileDialog.
 */
class OWL_API FileDialog {
public:
	/**
	 * @brief Open a file dialog to search for an existing file.
	 * @param filter Filter to apply during the search.
	 * @return A valid file path or null if canceled.
	 */
	static std::filesystem::path openFile(const std::string &filter);

	/**
	 * @brief Open a file dialog to define a file to create.
	 * @param filter Filter to apply during the search.
	 * @return A valid file path or null if canceled.
	 */
	static std::filesystem::path saveFile(const std::string &filter);
};

}// namespace owl::core
