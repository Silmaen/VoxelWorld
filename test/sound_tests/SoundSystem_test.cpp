
#include "testHelper.h"

#include <sound/SoundSystem.h>

using namespace owl::sound;

TEST(SoundSystem, creation) {
	owl::core::Log::init(spdlog::level::off);
	SoundSystem::reset();
	EXPECT_EQ(SoundSystem::getState(), SoundSystem::State::Created);
	SoundSystem::init();
	EXPECT_EQ(SoundSystem::getState(), SoundSystem::State::Error);
	SoundCommand::create(SoundAPI::Type::Null);
	SoundSystem::init();
	EXPECT_EQ(SoundSystem::getState(), SoundSystem::State::Running);
	SoundSystem::shutdown();
	EXPECT_EQ(SoundSystem::getState(), SoundSystem::State::Stopped);

	SoundCommand::invalidate();
	owl::core::Log::invalidate();
}

TEST(Renderer, fakeScene) {
	owl::core::Log::init(spdlog::level::off);
	SoundCommand::create(SoundAPI::Type::Null);
	SoundSystem::init();
	const owl::shared<SoundData> data = SoundData::create();
	SoundCommand::playSound(data);
	SoundCommand::invalidate();
	owl::core::Log::invalidate();
}
