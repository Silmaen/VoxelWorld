#include "testHelper.h"

#include <physic/PhysicCommand.h>
#include <scene/Entity.h>
#include <scene/Scene.h>
#include <scene/component/components.h>

using namespace owl::core;
using namespace owl::physic;
using namespace owl::scene;

TEST(PhysicCommand, Creation) {
	owl::core::Log::init(spdlog::level::off);
	Scene scene;
	// create a scene with physic

	auto b1 = scene.createEntity("body1");
	auto& phy1 = b1.addComponent<component::PhysicBody>();
	auto& col1 = b1.addComponent<component::PhysicCollider>();
	phy1.type = component::PhysicBody::BodyType::Dynamic;
	col1.restitution = 1.0f;
	auto& [transform1] = b1.getComponent<component::Transform>();
	transform1.translation().x() = 15.0f;

	auto b2 = scene.createEntity("body2");
	auto& phy2 = b2.addComponent<component::PhysicBody>();
	auto& col2 = b2.addComponent<component::PhysicCollider>();
	phy2.type = component::PhysicBody::BodyType::Static;
	col2.restitution = 1.0f;
	auto& [transform2] = b2.getComponent<component::Transform>();
	transform2.translation().x() = -15.0f;

	auto b3 = scene.createEntity("body3");
	auto& phy3 = b3.addComponent<component::PhysicBody>();
	phy3.type = component::PhysicBody::BodyType::Kinematic;
	auto& [transform3] = b3.getComponent<component::Transform>();
	transform3.translation().x() = 0.0f;

	Timestep ts;
	ts.forceUpdate(std::chrono::milliseconds(100));
	EXPECT_FALSE(PhysicCommand::isInitialized());
	PhysicCommand::frame(ts);
	PhysicCommand::init(nullptr);
	EXPECT_FALSE(PhysicCommand::isInitialized());
	PhysicCommand::init(&scene);
	EXPECT_TRUE(PhysicCommand::isInitialized());
	PhysicCommand::init(&scene);
	EXPECT_TRUE(PhysicCommand::isInitialized());
	PhysicCommand::frame(ts);
	// test the new positions...
	EXPECT_EQ(transform1.translation().y(), -0.0613125041f);
	EXPECT_EQ(transform2.translation().y(), 0.0f);
	EXPECT_EQ(transform3.translation().y(), 0.0f);

	PhysicCommand::destroy();
	EXPECT_FALSE(PhysicCommand::isInitialized());
	owl::core::Log::invalidate();
}
