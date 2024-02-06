
#include "testHelper.h"
#include <filesystem>
#include <renderer/RenderCommand.h>
#include <renderer/ShaderLibrary.h>

using namespace owl::renderer;

TEST(ShaderLibrary, creation) {
	owl::core::Log::init(spdlog::level::off);
	RenderCommand::create(RenderAPI::Type::Null);
	auto lib = ShaderLibrary();
	EXPECT_FALSE(lib.exists("bob", ""));
	EXPECT_FALSE(lib.get("bob", ""));
	EXPECT_TRUE(lib.load("bob", "", "bob"));
	lib.addFromStandardPath("bob2", "");
	EXPECT_FALSE(lib.exists("bob2", ""));
	const auto shader = Shader::create("superbob", "renderer");
	EXPECT_TRUE(shader);
	lib.add(shader);
	lib.add(shader);
	EXPECT_FALSE(lib.exists("superbob", ""));
	const auto shader2 = Shader::create("super", "renderer");
	lib.addNRename("superbob", "", shader2);
	lib.addNRename("boby", "", shader2);
	EXPECT_FALSE(lib.exists("super", ""));
	EXPECT_TRUE(lib.exists("superbob", ""));
	EXPECT_TRUE(lib.exists("boby", ""));
	RenderCommand::invalidate();
	owl::core::Log::invalidate();
}
