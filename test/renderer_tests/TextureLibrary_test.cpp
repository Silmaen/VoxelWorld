
#include "testHelper.h"

#include <core/Application.h>
#include <renderer/RenderCommand.h>
#include <renderer/TextureLibrary.h>

using namespace owl::renderer;
using namespace owl::core;

TEST(TextureLibrary, creation) {
	Log::init(spdlog::level::off);
	RenderCommand::create(RenderAPI::Type::Null);
	AppParams const params{.name = "super boby", .renderer = RenderAPI::Type::Null, .hasGui = false, .isDummy = true};
	auto app = owl::mkShared<Application>(params);
	auto lib = TextureLibrary();
	{
		EXPECT_FALSE(lib.exists("bob"));
		lib.addFromStandardPath("checkerboard");
		lib.addFromStandardPath("checkerboard");
		EXPECT_TRUE(lib.exists("checkerboard"));
		EXPECT_EQ(lib.get("bob"), nullptr);
		EXPECT_EQ(lib.get("checkerboard"), nullptr);
	}
	{
		auto bob = Texture2D::create({1, 1}, false);
		lib.addNRename("checkerboard", bob);
		lib.addNRename("superbob", bob);
		EXPECT_TRUE(lib.exists("superbob"));
	}
	{
		auto bob = Texture2D::create({1, 1}, false);
		lib.load("checkerboard");
		lib.add(bob);
		EXPECT_TRUE(lib.exists(""));
		lib.add(bob);
	}
	Application::invalidate();
	app.reset();
	RenderCommand::invalidate();
	Log::invalidate();
}
