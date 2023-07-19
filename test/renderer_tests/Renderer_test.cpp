
#include "testHelper.h"
#include <filesystem>
#include <renderer/Renderer.h>

using namespace owl::renderer;

TEST(Renderer, creation) {
	owl::core::Log::init(spdlog::level::off);
	Renderer::reset();
	EXPECT_EQ(Renderer::getState(), Renderer::State::Created);
	Renderer::init();
	EXPECT_EQ(Renderer::getState(), Renderer::State::Error);
	RenderCommand::create(RenderAPI::Type::Null);
	Renderer::init();
	EXPECT_EQ(Renderer::getState(), Renderer::State::Running);
	Renderer::onWindowResized(800, 600);
	Renderer::shutdown();
	EXPECT_EQ(Renderer::getState(), Renderer::State::Stopped);

	RenderCommand::invalidate();
	owl::core::Log::invalidate();
}

TEST(Renderer, fakeScene) {
	owl::core::Log::init(spdlog::level::off);
	RenderCommand::create(RenderAPI::Type::Null);
	Renderer::init();
	CameraOrtho cam(0, 0, 800, 600);
	Renderer::beginScene(cam);
	auto obj = VertexArray::create();
	Renderer::submit("toto", obj);
	auto shad = Shader::create(std::string("bob"));
	Renderer::submit(shad, obj);
	Renderer::endScene();

	RenderCommand::invalidate();
	owl::core::Log::invalidate();
}
