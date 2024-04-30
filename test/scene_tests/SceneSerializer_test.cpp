
#include "renderer/null/Texture.h"
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
	auto &spr = ent2.addOrReplaceComponent<component::SpriteRenderer>();
	spr.texture = owl::mkShared<owl::renderer::null::Texture2D>(1, 1);
	spr.tilingFactor = 12.3f;

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

TEST(SceneSerializer, badScene) {
	owl::core::Log::init(spdlog::level::off);
	const auto fs = std::filesystem::temp_directory_path() / "tempSave.yml";

	// write bad file
	{
		std::ofstream file(fs);
		file << "Bob: toto" << std::endl;
		file.close();
		const auto sc = owl::mkShared<Scene>();
		const SceneSerializer loader(sc);
		EXPECT_FALSE(loader.deserialize(fs));
		remove(fs);
	}
	// write bad file
	{
		std::ofstream file(fs);
		file << "Scene: untitled" << std::endl;
		file << "  - je suis une fougère.:" << std::endl;
		file.close();
		const auto sc = owl::mkShared<Scene>();
		const SceneSerializer loader(sc);
		EXPECT_FALSE(loader.deserialize(fs));
		remove(fs);
	}
	// write another bad file
	{
		std::ofstream file(fs);
		file << "Scene: untitled" << std::endl;
		file << "Entities:" << std::endl;
		file << "  - Entity: 1" << std::endl;
		file << "  - Entity: 2" << std::endl;
		file << "    SpriteRenderer:" << std::endl;
		file << "      color: 0.949019611" << std::endl;
		file << "  - Entity: 3" << std::endl;
		file.close();
		const auto sc = owl::mkShared<Scene>();
		const SceneSerializer loader(sc);
		EXPECT_FALSE(loader.deserialize(fs));
		remove(fs);
	}
	// write another bad file
	{
		std::ofstream file(fs);
		file << "Scene: untitled" << std::endl;
		file << "Entities:" << std::endl;
		file << "  - Entity: 1" << std::endl;
		file << "  - Entity: 2" << std::endl;
		file << "    Transform:" << std::endl;
		file << "      translation: 0.353553385" << std::endl;
		file.close();
		const auto sc = owl::mkShared<Scene>();
		const SceneSerializer loader(sc);
		EXPECT_FALSE(loader.deserialize(fs));
		remove(fs);
	}
	// write another bad file
	{
		std::ofstream file(fs);
		file << "Scene: untitled" << std::endl;
		file << "Entities:" << std::endl;
		file << "  - Entity: 1" << std::endl;
		file << "  - Entity: 3" << std::endl;
		file << "    Transform:" << std::endl;
		file << "      translation: [0.353553385, 0]" << std::endl;
		file.close();
		const auto sc = owl::mkShared<Scene>();
		const SceneSerializer loader(sc);
		EXPECT_FALSE(loader.deserialize(fs));
		remove(fs);
	}
	// write another bad file
	{
		std::ofstream file(fs);
		file << "Scene: untitled" << std::endl;
		file << "Entities:" << std::endl;
		file << "  - Entity: 1" << std::endl;
		file << "  - Entity: 3" << std::endl;
		file << "    SpriteRenderer:" << std::endl;
		file << "      color: [0, 0.949019611]" << std::endl;
		file.close();
		const auto sc = owl::mkShared<Scene>();
		const SceneSerializer loader(sc);
		EXPECT_FALSE(loader.deserialize(fs));
		remove(fs);
	}
	EXPECT_FALSE(exists(fs));
	owl::core::Log::invalidate();
}
