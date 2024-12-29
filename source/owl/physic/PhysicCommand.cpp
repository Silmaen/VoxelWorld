/**
 * @file PhysicCommand.cpp
 * @author Silmaen
 * @date 12/27/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "PhysicCommand.h"
#include "scene/Entity.h"
#include "scene/component/components.h"
#include <box2d.h>

namespace owl::physic {

class PhysicCommand::Impl {
public:
	Impl() {}
	~Impl() {}

	b2WorldId worldId{0, 0};
	uint64_t nextId = 1;
	std::unordered_map<uint64_t, b2BodyId> bodies;
};

shared<PhysicCommand::Impl> PhysicCommand::m_impl = std::make_shared<Impl>();
scene::Scene* PhysicCommand::m_scene = nullptr;

PhysicCommand::PhysicCommand() = default;

void PhysicCommand::init(scene::Scene* iScene) {
	if (iScene == nullptr)
		return;
	if (isInitialized())
		destroy();
	m_scene = iScene;
	b2WorldDef def = b2DefaultWorldDef();
	def.gravity = {0.0f, -9.81f};
	m_impl->worldId = b2CreateWorld(&def);
	OWL_INFO("PhysicCommand::init(), world created ({} {})", m_impl->worldId.index1, m_impl->worldId.revision)

	// Add entities...
	for (const auto& view = m_scene->registry.view<scene::component::PhysicBody, scene::component::Transform>();
		 const auto& e: view) {
		const scene::Entity entity{e, m_scene};
		auto& [type, fixedRotation, bodyId] = entity.getComponent<scene::component::PhysicBody>();
		auto& [transform] = entity.getComponent<scene::component::Transform>();
		b2BodyDef bodyDef = b2DefaultBodyDef();
		switch (type) {
			case scene::component::PhysicBody::BodyType::Static:
				bodyDef.type = b2_staticBody;
				break;
			case scene::component::PhysicBody::BodyType::Dynamic:
				bodyDef.type = b2_dynamicBody;
				break;
			case scene::component::PhysicBody::BodyType::Kinematic:
				bodyDef.type = b2_kinematicBody;
				break;
		}
		bodyDef.fixedRotation = fixedRotation;
		bodyDef.position.x = transform.translation().x();
		bodyDef.position.y = transform.translation().y();
		bodyDef.rotation = b2MakeRot(transform.rotation().z());

		const b2BodyId body = b2CreateBody(m_impl->worldId, &bodyDef);
		OWL_INFO("PhysicCommand::init(), body created ({} {} {})", body.index1, body.world0, body.revision)
		bodyId = m_impl->nextId;
		m_impl->bodies[m_impl->nextId] = body;
		m_impl->nextId++;

		if (entity.hasComponent<scene::component::PhysicCollider>()) {
			auto& [size, density, restitution, friction] = entity.getComponent<scene::component::PhysicCollider>();
			const b2Polygon dynamicBox =
					b2MakeBox(size.x() * transform.scale().x() * 0.5f, size.y() * transform.scale().y() * 0.5f);
			b2ShapeDef shapeDef = b2DefaultShapeDef();
			shapeDef.density = density;
			shapeDef.friction = friction;
			shapeDef.restitution = restitution;
			b2CreatePolygonShape(body, &shapeDef, &dynamicBox);
		}
	}
}

void PhysicCommand::destroy() {
	m_scene = nullptr;
	b2DestroyWorld(m_impl->worldId);
	m_impl->worldId = {.index1 = 0, .revision = 0};
	m_impl->bodies.clear();
}

auto PhysicCommand::isInitialized() -> bool {
	if (m_scene == nullptr)
		return false;
	return true;
}

void PhysicCommand::frame(const core::Timestep& iTimestep) {
	if (!isInitialized())
		return;
	// Update the physical world
	b2World_Step(m_impl->worldId, iTimestep.getSeconds(), 4);

	// apply to the entities
	for (const auto view = m_scene->registry.view<scene::component::Transform, scene::component::PhysicBody>();
		 const auto entity: view) {
		auto&& [transform, physic] = view.get<scene::component::Transform, scene::component::PhysicBody>(entity);
		auto [x, y] = b2Body_GetPosition(m_impl->bodies[physic.bodyId]);
		transform.transform.translation().x() = x;
		transform.transform.translation().y() = y;
		transform.transform.rotation().z() = b2Rot_GetAngle(b2Body_GetRotation(m_impl->bodies[physic.bodyId]));
	}
}


}// namespace owl::physic
