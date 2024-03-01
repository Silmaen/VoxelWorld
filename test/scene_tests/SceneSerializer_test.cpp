
#include "testHelper.h"

#include <renderer/Texture.h>
#include <scene/Entity.h>
#include <scene/Scene.h>
#include <scene/SceneSerializer.h>
#include <scene/component/Camera.h>
#include <scene/component/CircleRenderer.h>
#include <scene/component/SpriteRenderer.h>
#include <scene/component/Transform.h>

using namespace owl::scene;

class MyTexture2D final : public owl::renderer::Texture2D {
public:
	MyTexture2D() : Texture2D() {}
	[[nodiscard]] bool operator==(const Texture &iOther) const override { return p == iOther.getPath(); }
	[[nodiscard]] uint32_t getWidth() const override { return 0; }
	[[nodiscard]] uint32_t getHeight() const override { return 0; }
	[[nodiscard]] owl::math::FrameSize getSize() const override { return {0, 0}; }
	[[nodiscard]] bool isLoaded() const override { return true; }
	[[nodiscard]] uint32_t getRendererId() const override { return 0; }
	void bind(uint32_t) const override {}
	[[nodiscard]] const std::filesystem::path &getPath() const override { return p; }
	void setData(void *, uint32_t) override {}

private:
	std::filesystem::path p{""};
};

TEST(SceneSerializer, SaveLoad) {
	owl::core::Log::init(spdlog::level::off);
	const auto sc = owl::mkShared<Scene>();
	sc->createEntityWithUUID(5, "bobObject");
	const SceneSerializer saver(sc);
	const auto fs = std::filesystem::temp_directory_path() / "tempSave.yml";
	saver.serialize(fs);

	ASSERT_TRUE(exists(fs));
	const auto sc2 = owl::mkShared<Scene>();
	const SceneSerializer loader(sc2);
	EXPECT_TRUE(loader.deserialize(fs));

	EXPECT_EQ(sc2->registry.storage<owl::scene::Entity>().size(), sc->registry.storage<owl::scene::Entity>().size());
	remove(fs);
	EXPECT_FALSE(exists(fs));
	owl::core::Log::invalidate();
}

TEST(SceneSerializer, SaveLoadFULL) {
	owl::core::Log::init(spdlog::level::off);
	const auto sc = owl::mkShared<Scene>();
	auto ent = sc->createEntityWithUUID(5, "bobObject");
	ent.addOrReplaceComponent<component::Camera>();
	ent.addOrReplaceComponent<component::CircleRenderer>();
	ent.addOrReplaceComponent<component::SpriteRenderer>();
	auto ent2 = sc->createEntityWithUUID(7, "bobObject2");
	auto spr = ent2.addOrReplaceComponent<component::SpriteRenderer>();
	spr.texture = owl::mkShared<MyTexture2D>();

	const SceneSerializer saver(sc);
	const auto fs = std::filesystem::temp_directory_path() / "tempSave.yml";
	saver.serialize(fs);

	ASSERT_TRUE(exists(fs));
	const auto sc2 = owl::mkShared<Scene>();
	const SceneSerializer loader(sc2);
	EXPECT_TRUE(loader.deserialize(fs));

	EXPECT_EQ(sc2->registry.storage<owl::scene::Entity>().size(), sc->registry.storage<owl::scene::Entity>().size());
	remove(fs);
	EXPECT_FALSE(exists(fs));
	owl::core::Log::invalidate();
}
