
#include "testHelper.h"

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
TEST(Shader, basis) {
	owl::core::Log::init(spdlog::level::off);
	RenderCommand::create(RenderAPI::Type::Null);
	const auto shader2 = Shader::create("super", "renderer", "");
	shader2->bind();
	shader2->unbind();
	int ii[3] = {0, 1, 2};
	shader2->setIntArray("bob", ii, 3);
	shader2->setInt("fbob", 333);
	shader2->setFloat("fbob", 0.1f);
	shader2->setFloat2("fbob", {2, 3});
	shader2->setFloat3("fbob", {9, 8, 7});
	shader2->setFloat4("fbob", {255, 255, 45, 255});
	shader2->setMat4("fbob", {255, 255, 45, 255, 255, 255, 45, 255, 255, 255, 45, 255, 255, 255, 45, 255});

	EXPECT_STREQ(shader2->getFullName().c_str(), "renderer_super");
	RenderCommand::invalidate();
	owl::core::Log::invalidate();
}
