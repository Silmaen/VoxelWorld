/**
 * @file PlatformUtils.cpp
 * @author Silmaen
 * @date 27/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "core/Core.h"
#include "core/PlatformUtils.h"

#include "core/Application.h"

#include <nfd.hpp>

namespace owl::core {

static std::vector<std::string_view> split(const std::string_view str, const char delim = '\n') {
	std::vector<std::string_view> result;
	int indexCommaToLeftOfColumn = 0;
	int indexCommaToRightOfColumn = -1;
	for (uint32_t i = 0; i < str.size(); i++) {
		if (str[i] == delim) {
			indexCommaToLeftOfColumn = indexCommaToRightOfColumn;
			indexCommaToRightOfColumn = static_cast<int>(i);
			int32_t index = indexCommaToLeftOfColumn + 1;
			uint32_t length = static_cast<uint32_t>(indexCommaToRightOfColumn - index);
			std::string_view column(str.data() + index, length);
			result.push_back(column);
		}
	}
	const std::string_view finalColumn(str.data() + indexCommaToRightOfColumn + 1, str.size() - static_cast<size_t>(indexCommaToRightOfColumn - 1));
	result.push_back(finalColumn);
	return result;
}

static std::vector<nfdu8filteritem_t> parseFilter(std::string &filter) {
	std::vector<nfdu8filteritem_t> filters;
	auto filterStr = split(filter);
	for (auto str: filterStr) {
		if (str.empty()) continue;
		auto items = split(str, '|');
		if (items.size() != 2) continue;
		if (items[0].empty() || items[1].empty()) continue;
		filters.push_back({items[0].data(), items[1].data()});
	}
	for (auto &c: filter) {
		if (c == '|') c = '\0';
		if (c == '\n') c = '\0';
	}
	return filters;
}

std::filesystem::path FileDialog::openFile(const std::string &filter) {
	NFD::Init();
	nfdu8char_t *outpath;
	std::filesystem::path resultPath;
	std::string cfilters{filter};
	auto ff = parseFilter(cfilters);
	auto initialDir = Application::get().getAssetDirectory();
	std::string tmp = initialDir.string();
	auto result = NFD::OpenDialog(outpath, ff.data(), static_cast<uint32_t>(ff.size()), tmp.c_str());
	if (result == NFD_CANCEL) {
		resultPath = std::filesystem::path{};
	} else if (result == NFD_OKAY) {
		resultPath = std::filesystem::path{outpath};
		NFD_FreePath(outpath);
	} else {
		OWL_CORE_ERROR("while opening file: {}", NFD::GetError())
		OWL_ASSERT(false, "Error Opening file")
	}
	NFD::Quit();
	return resultPath;
}

std::filesystem::path FileDialog::saveFile([[maybe_unused]] const std::string &filter) {
	NFD::Init();
	nfdu8char_t *outpath;
	std::filesystem::path resultPath;
	std::string cfilters{filter};
	auto ff = parseFilter(cfilters);
	auto initialDir = Application::get().getAssetDirectory();
	auto result = NFD::SaveDialog(outpath, ff.data(), static_cast<uint32_t>(ff.size()), initialDir.string().c_str(), nullptr);
	if (result == NFD_CANCEL) {
		resultPath = std::filesystem::path{};
	} else if (result == NFD_OKAY) {
		resultPath = std::filesystem::path{outpath};
	} else {
		OWL_CORE_ERROR("while opening file: {}", NFD::GetError())
		OWL_ASSERT(false, "Error Opening file")
	}
	NFD::Quit();
	return resultPath;
}

}// namespace owl::core
