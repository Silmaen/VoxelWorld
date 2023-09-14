
#include "testHelper.h"

#include <filesystem>
#include <renderer/Framebuffer.h>
#include <renderer/GraphContext.h>
#include <renderer/RenderAPI.h>
#include <renderer/RenderCommand.h>
#include <renderer/Shader.h>
#include <renderer/Texture.h>
#include <renderer/UniformBuffer.h>

using namespace owl::renderer;

TEST(RenderAPI, RenderComamand) {
	owl::core::Log::init(spdlog::level::off);
	RenderCommand::create(RenderAPI::Type{-1});
	EXPECT_EQ(RenderCommand::getState(), RenderAPI::State::Error);
	RenderCommand::create(RenderAPI::Type::Vulkan);
	EXPECT_EQ(RenderCommand::getState(), RenderAPI::State::Error);
	RenderCommand::create(RenderAPI::Type::Null);
	EXPECT_EQ(RenderCommand::getState(), RenderAPI::State::Created);
	EXPECT_EQ(RenderCommand::getAPI(), RenderAPI::Type::Null);
	RenderCommand::setLineWidth(2.f);
	RenderCommand::setClearColor({.4f, .4f, .4f, 1.f});
	RenderCommand::clear();
	RenderCommand::init();
	EXPECT_EQ(RenderCommand::getState(), RenderAPI::State::Ready);
	RenderCommand::setViewport(1, 1, 45, 65);
	RenderCommand::invalidate();
	EXPECT_EQ(RenderCommand::getState(), RenderAPI::State::Error);
	owl::core::Log::invalidate();
}

TEST(RenderAPI, badCreation) {
	// bad Type value
	owl::core::Log::init(spdlog::level::off);
	owl::uniq<RenderAPI> api = RenderAPI::create(RenderAPI::Type{-1});
	auto gf = GraphContext::create(nullptr);
	auto fb = Framebuffer::create({});
	auto ub = UniformBuffer::create(0, 0);
	auto tex = Texture2D::create(std::filesystem::path());
	auto tex2 = Texture2D::create(1, 1);
	auto shader = Shader::create(std::filesystem::path());
	auto shader2 = Shader::create(std::string("bob"));
	auto shader3 = Shader::create("bob2", "", "");
	EXPECT_TRUE(api == nullptr);
	EXPECT_TRUE(gf == nullptr);
	EXPECT_TRUE(fb == nullptr);
	EXPECT_TRUE(ub == nullptr);
	EXPECT_TRUE(tex == nullptr);
	EXPECT_TRUE(tex2 == nullptr);
	EXPECT_TRUE(shader == nullptr);
	EXPECT_TRUE(shader2 == nullptr);
	EXPECT_TRUE(shader3 == nullptr);
	// vulkan not implemented
	api = RenderAPI::create(RenderAPI::Type::Vulkan);
	gf = GraphContext::create(nullptr);
	fb = Framebuffer::create({});
	ub = UniformBuffer::create(0, 0);
	tex = Texture2D::create(std::filesystem::path());
	tex2 = Texture2D::create(1, 1);
	shader = Shader::create(std::filesystem::path());
	shader2 = Shader::create(std::string("bob"));
	shader3 = Shader::create("bob2", "", "");
	EXPECT_TRUE(api == nullptr);
	EXPECT_TRUE(gf == nullptr);
	EXPECT_TRUE(fb == nullptr);
	EXPECT_TRUE(ub == nullptr);
	EXPECT_TRUE(tex == nullptr);
	EXPECT_TRUE(tex2 == nullptr);
	EXPECT_TRUE(shader == nullptr);
	EXPECT_TRUE(shader2 == nullptr);
	EXPECT_TRUE(shader3 == nullptr);
	owl::core::Log::invalidate();
}

TEST(RenderAPI, creation) {
	owl::core::Log::init(spdlog::level::off);
	auto api = RenderAPI::create(RenderAPI::Type::Null);
	ASSERT_TRUE(api);
	EXPECT_EQ(RenderAPI::getState(), RenderAPI::State::Created);
	api->init();
	EXPECT_EQ(RenderAPI::getState(), RenderAPI::State::Ready);
	EXPECT_EQ(RenderAPI::getAPI(), RenderAPI::Type::Null);
	owl::core::Log::invalidate();
}
