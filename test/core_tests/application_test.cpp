
#include "testHelper.h"

#include <core/Application.h>
#include <core/utils/FileUtils.h>
#include <fstream>

using namespace owl::core;

TEST_DISABLED(Core, Application_dummy) {
	AppParams params;
	params.renderer = owl::renderer::RenderAPI::Type::Null;
	auto app = owl::mk_shared<Application>(params);
	EXPECT_EQ(app->getState(), Application::State::Error);
	Application::invalidate();
	app.reset();
}

TEST_DISABLED(Core, Application_basic) {
	AppParams params;
	auto app = owl::mk_shared<Application>(params);
	EXPECT_EQ(app->getAssetDirectory().filename(), "assets");
	EXPECT_FALSE(app->getWorkingDirectory().filename().string().empty());
	EXPECT_EQ(app->getState(), Application::State::Running);
	app->enableDocking();
	app->disableDocking();
	app->close();
	EXPECT_EQ(app->getState(), Application::State::Stopped);
	Application::invalidate();
	app.reset();
}

TEST(Core, AppParams_serialize) {
	AppParams paramsIni;
	paramsIni.width = 785;
	paramsIni.height = 7415;
	paramsIni.renderer = owl::renderer::RenderAPI::Type::Vulkan;
	auto tmpFile = std::filesystem::temp_directory_path() / "param.yml";
	paramsIni.saveToFile(tmpFile);

	AppParams paramsLoad;
	EXPECT_NE(paramsIni.width, paramsLoad.width);
	EXPECT_NE(paramsIni.height, paramsLoad.height);
	EXPECT_NE(paramsIni.renderer, paramsLoad.renderer);
	paramsLoad.loadFromFile(tmpFile);
	EXPECT_EQ(paramsIni.width, paramsLoad.width);
	EXPECT_EQ(paramsIni.height, paramsLoad.height);
	EXPECT_EQ(paramsIni.renderer, paramsLoad.renderer);
	remove(tmpFile);
}

TEST(Core, fileToString) {
	Log::setVerbosityLevel(spdlog::level::off);
	Log::init();
	const std::string superStr = "yoho\nI'm fine!\n";
	const auto tmpFile = std::filesystem::temp_directory_path() / "testFile.b";
	{
		std::ofstream out(tmpFile, std::ios::out);
		out << superStr;
		out.close();
	}
	std::string read = utils::fileToString(tmpFile);
	EXPECT_STREQ(superStr.c_str(), read.c_str());
	read = utils::fileToString("");
	EXPECT_STREQ("", read.c_str());
	remove(tmpFile);
	Log::invalidate();
}
