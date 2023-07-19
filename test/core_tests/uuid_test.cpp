
#include "testHelper.h"
#include <core/UUID.h>

using namespace owl::core;

TEST(UUID, generate) {
	UUID uuid{};
	EXPECT_NE(uuid, 0);
}

TEST(UUID, uint64_test) {
	uint64_t id = 666;
	UUID uuid(id);
	EXPECT_EQ(id, uuid);
	EXPECT_EQ(std::hash<UUID>{}(uuid), 666);
}
