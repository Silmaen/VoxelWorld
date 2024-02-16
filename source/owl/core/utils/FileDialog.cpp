/**
 * @file FileDialog.cpp
 * @author Silmaen
 * @date 27/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "FileDialog.h"
#include "core/Application.h"

#include <nfd.hpp>

namespace owl::core::utils {

static std::vector<std::string_view> split(const std::string_view str, const char delimiter = '\n') {
	std::vector<std::string_view> result;
	int indexCommaToRightOfColumn = -1;
	for (uint32_t i = 0; i < str.size(); i++) {
		if (str[i] == delimiter) {
			int indexCommaToLeftOfColumn = indexCommaToRightOfColumn;
			indexCommaToRightOfColumn = static_cast<int>(i);
			int32_t index = indexCommaToLeftOfColumn + 1;
			auto length = static_cast<uint32_t>(indexCommaToRightOfColumn - index);
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
	nfdu8char_t *outPath;
	std::filesystem::path resultPath;
	std::string filters{filter};
	auto ff = parseFilter(filters);
	auto initialDir = Application::get().getAssetDirectory();
	std::string tmp = initialDir.string();
	auto result = NFD::OpenDialog(outPath, ff.data(), static_cast<uint32_t>(ff.size()), tmp.c_str());
	if (result == NFD_CANCEL) {
		resultPath = std::filesystem::path{};
	} else if (result == NFD_OKAY) {
		resultPath = std::filesystem::path{outPath};
		NFD_FreePath(outPath);
	} else {
		OWL_CORE_ERROR("while opening file: {}", NFD::GetError())
		OWL_ASSERT(false, "Error Opening file")
	}
	NFD::Quit();
	return resultPath;
}

std::filesystem::path FileDialog::saveFile([[maybe_unused]] const std::string &filter) {
	NFD::Init();
	nfdu8char_t *outPath;
	std::filesystem::path resultPath;
	std::string filters{filter};
	auto ff = parseFilter(filters);
	auto initialDir = Application::get().getAssetDirectory();
	auto result = NFD::SaveDialog(outPath, ff.data(), static_cast<uint32_t>(ff.size()), initialDir.string().c_str(), nullptr);
	if (result == NFD_CANCEL) {
		resultPath = std::filesystem::path{};
	} else if (result == NFD_OKAY) {
		resultPath = std::filesystem::path{outPath};
	} else {
		OWL_CORE_ERROR("while opening file: {}", NFD::GetError())
		OWL_ASSERT(false, "Error Opening file")
	}
	NFD::Quit();
	return resultPath;
}

}// namespace owl::core::utils
