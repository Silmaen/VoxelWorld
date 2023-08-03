
#include "testHelper.h"
#include <filesystem>
#include <renderer/Texture.h>
#include <scene/Entity.h>
#include <scene/Scene.h>
#include <scene/SceneSerializer.h>
#include <scene/component/Camera.h>
#include <scene/component/CircleRenderer.h>
#include <scene/component/NativeScript.h>
#include <scene/component/SpriteRenderer.h>
#include <scene/component/Transform.h>

using namespace owl::scene;

class myTexture2D : public owl::renderer::Texture2D {
public:
	myTexture2D() : owl::renderer::Texture2D() {}
	[[nodiscard]] bool operator==(const owl::renderer::Texture &other) const override { return p == other.getPath(); }
	[[nodiscard]] uint32_t getWidth() const override { return 0; }
	[[nodiscard]] uint32_t getHeight() const override { return 0; }
	[[nodiscard]] bool isLoaded() const override { return true; }
	[[nodiscard]] uint32_t getRendererID() const override { return 0; }
	void bind(uint32_t) const override {}
	[[nodiscard]] const std::filesystem::path &getPath() const override { return p; }
	void setData(void *, uint32_t) override {}

private:
	std::filesystem::path p{""};
};

TEST(SceneSerializer, SaveLoad) {
	owl::core::Log::init(spdlog::level::off);
	auto sc = owl::mk_shared<Scene>();
	sc->createEntityWithUUID(5, "bobObject");
	SceneSerializer saver(sc);
	auto fs = std::filesystem::temp_directory_path() / "tempSave.yml";
	saver.serialize(fs);

	ASSERT_TRUE(exists(fs));
	auto sc2 = owl::mk_shared<Scene>();
	SceneSerializer loader(sc2);
	loader.deserialize(fs);

	EXPECT_EQ(sc2->registry.size(), sc->registry.size());
	remove(fs);
	EXPECT_FALSE(exists(fs));
	owl::core::Log::invalidate();
}

TEST(SceneSerializer, SaveLoadFULL) {
	owl::core::Log::init(spdlog::level::off);
	auto sc = owl::mk_shared<Scene>();
	auto ent = sc->createEntityWithUUID(5, "bobObject");
	ent.addOrReplaceComponent<component::Camera>();
	ent.addOrReplaceComponent<component::CircleRenderer>();
	ent.addOrReplaceComponent<component::SpriteRenderer>();
	auto ent2 = sc->createEntityWithUUID(7, "bobObject2");
	auto spr = ent.addOrReplaceComponent<component::SpriteRenderer>();
	spr.texture = owl::mk_shared<myTexture2D>();

	SceneSerializer saver(sc);
	auto fs = std::filesystem::temp_directory_path() / "tempSave.yml";
	saver.serialize(fs);

	ASSERT_TRUE(exists(fs));
	auto sc2 = owl::mk_shared<Scene>();
	SceneSerializer loader(sc2);
	loader.deserialize(fs);

	EXPECT_EQ(sc2->registry.size(), sc->registry.size());
	remove(fs);
	EXPECT_FALSE(exists(fs));
	owl::core::Log::invalidate();
}
