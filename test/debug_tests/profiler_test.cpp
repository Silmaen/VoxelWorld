
#include "debug/Profiler.h"
#include "testHelper.h"
#include <filesystem>

using namespace owl::debug;

TEST(profiler, creation) {
	auto &prof = Profiler::get();
	prof.beginSession("bob", "");
	prof.endSession();
	owl::core::Log::init(spdlog::level::off);
	prof.beginSession("bob", "");
	prof.endSession();
	std::filesystem::path file("test_profile.json");
	prof.beginSession("bob2", file.string());
	std::filesystem::path file2("test_profile2.json");
	prof.beginSession("bob2", file.string());
	prof.endSession();
	EXPECT_TRUE(exists(file));
	EXPECT_FALSE(exists(file2));
	remove(file);
	remove(file2);
	owl::core::Log::invalidate();
}
