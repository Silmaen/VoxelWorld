/**
 * @file ContentBrowser.h
 * @author Silmaen
 * @date 10/01/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <filesystem>

#include "renderer/Texture.h"

namespace owl::panel {

/**
 * @brief Class ContentBrowser
 */
class ContentBrowser {
public:
	/**
	 * @brief Default copy constructor
	 */
	ContentBrowser(const ContentBrowser &) = default;
	/**
	 * @brief Default move constructor
	 */
	ContentBrowser(ContentBrowser &&) = default;
	/**
	 * @brief Default copy assignation
	 * @return this
	 */
	ContentBrowser &operator=(const ContentBrowser &) = default;
	/**
	 * @brief Default move assignation
	 * @return this
	 */
	ContentBrowser &operator=(ContentBrowser &&) = default;
	/**
	 * @brief Default constructor.
	 */
	ContentBrowser();
	/**
	 * @brief Destructor.
	 */
	~ContentBrowser() = default;

	/**
	 * @brief The render command
	 */
	void onImGuiRender();

private:
	/// The actual folder
	std::filesystem::path currentPath;

	shared<renderer::Texture2D> fileIcon;
	shared<renderer::Texture2D> dirIcon;
};

}// namespace owl::panel
