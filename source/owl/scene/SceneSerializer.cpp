/**
* @file SceneSerializer.cpp
* @author Silmaen
* @date 27/12/2022
* Copyright © 2022 All rights reserved.
* All modification must get authorization from the author.
*/
#include "owlpch.h"

#include "SceneSerializer.h"

#include "Entity.h"
#include "scene/component/Camera.h"
#include "scene/component/CircleRenderer.h"
#include "scene/component/SpriteRenderer.h"
#include "scene/component/Tag.h"
#include "scene/component/Transform.h"

#include "core/external/yaml.h"

#include <magic_enum.hpp>


namespace YAML {

template<>
struct convert<glm::vec3> {
	static Node encode(const glm::vec3 &rhs) {
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.SetStyle(EmitterStyle::Flow);
		return node;
	}
	static bool decode(const Node &node, glm::vec3 &rhs) {
		if (!node.IsSequence() || node.size() != 3)
			return false;
		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		return true;
	}
};

template<>
struct convert<glm::vec4> {
	static Node encode(const glm::vec4 &rhs) {
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.push_back(rhs.w);
		node.SetStyle(EmitterStyle::Flow);
		return node;
	}
	static bool decode(const Node &node, glm::vec4 &rhs) {
		if (!node.IsSequence() || node.size() != 4)
			return false;
		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		rhs.w = node[3].as<float>();
		return true;
	}
};

static Emitter &operator<<(Emitter &out, const glm::vec3 &v) {
	out << Flow;
	out << BeginSeq << v.x << v.y << v.z << EndSeq;
	return out;
}

static Emitter &operator<<(Emitter &out, const glm::vec4 &v) {
	out << Flow;
	out << BeginSeq << v.x << v.y << v.z << v.w << EndSeq;
	return out;
}

}// namespace YAML

namespace owl::scene {

SceneSerializer::SceneSerializer(const shared<Scene> &scene_) : scene(scene_) {
}

static void serializeEntity(YAML::Emitter &out, Entity entity) {
	out << YAML::BeginMap;// Entity
	out << YAML::Key << "Entity" << YAML::Value << entity.getUUID();

	if (entity.hasComponent<component::Tag>()) {
		out << YAML::Key << "Tag";
		out << YAML::BeginMap;// Tag
		auto &tag = entity.getComponent<component::Tag>().tag;
		out << YAML::Key << "tag" << YAML::Value << tag;
		out << YAML::EndMap;// Tag
	}

	if (entity.hasComponent<component::Transform>()) {
		out << YAML::Key << "Transform";
		out << YAML::BeginMap;// Transform
		auto &tc = entity.getComponent<component::Transform>();
		out << YAML::Key << "translation" << YAML::Value << tc.translation;
		out << YAML::Key << "rotation" << YAML::Value << tc.rotation;
		out << YAML::Key << "scale" << YAML::Value << tc.scale;
		out << YAML::EndMap;// Transform
	}

	if (entity.hasComponent<component::Camera>()) {
		out << YAML::Key << "Camera";
		out << YAML::BeginMap;// CameraComponent
		auto &cameraComponent = entity.getComponent<component::Camera>();
		auto &camera = cameraComponent.camera;
		out << YAML::Key << "camera" << YAML::Value;
		out << YAML::BeginMap;// Camera
		out << YAML::Key << "projectionType" << YAML::Value << std::string(magic_enum::enum_name(camera.getProjectionType()));
		out << YAML::Key << "perspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
		out << YAML::Key << "perspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
		out << YAML::Key << "perspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
		out << YAML::Key << "orthographicSize" << YAML::Value << camera.getOrthographicSize();
		out << YAML::Key << "orthographicNear" << YAML::Value << camera.getOrthographicNearClip();
		out << YAML::Key << "orthographicFar" << YAML::Value << camera.getOrthographicFarClip();
		out << YAML::EndMap;// Camera
		out << YAML::Key << "primary" << YAML::Value << cameraComponent.primary;
		out << YAML::Key << "fixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;
		out << YAML::EndMap;// Camera
	}

	if (entity.hasComponent<component::SpriteRenderer>()) {
		out << YAML::Key << "SpriteRenderer";
		out << YAML::BeginMap;// SpriteRenderer
		auto &spriteRendererComponent = entity.getComponent<component::SpriteRenderer>();
		out << YAML::Key << "color" << YAML::Value << spriteRendererComponent.color;
		if (spriteRendererComponent.texture) {
			out << YAML::Key << "tilingFactor" << YAML::Value << spriteRendererComponent.tilingFactor;
			out << YAML::Key << "texture" << YAML::Value << spriteRendererComponent.texture->getPath().string();
		}
		out << YAML::EndMap;// SpriteRenderer
	}
	if (entity.hasComponent<component::CircleRenderer>()) {
		out << YAML::Key << "CircleRenderer";
		out << YAML::BeginMap;// CircleRenderer
		auto &circleRendererComponent = entity.getComponent<component::CircleRenderer>();
		out << YAML::Key << "color" << YAML::Value << circleRendererComponent.color;
		out << YAML::Key << "thickness" << YAML::Value << circleRendererComponent.thickness;
		out << YAML::Key << "fade" << YAML::Value << circleRendererComponent.fade;
		out << YAML::EndMap;// CircleRenderer
	}

	out << YAML::EndMap;// Entity
}

void SceneSerializer::serialize(const std::filesystem::path &filepath) {
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "untitled";
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	scene->registry.each([&](auto entityID) {
		Entity entity = {entityID, scene.get()};
		if (!entity)
			return;
		serializeEntity(out, entity);
	});
	out << YAML::EndSeq;
	out << YAML::EndMap;
	std::ofstream fileOut(filepath);
	fileOut << out.c_str();
	fileOut.close();
}

bool SceneSerializer::deserialize(const std::filesystem::path &filepath) {
	YAML::Node data = YAML::LoadFile(filepath.string());
	if (!data["Scene"])
		return false;
	auto sceneName = data["Scene"].as<std::string>();
	OWL_CORE_TRACE("Deserializing scene '{0}'", sceneName)
	auto entities = data["Entities"];
	if (entities) {
		for (auto entity: entities) {
			auto uuid = entity["Entity"].as<uint64_t>();
			std::string name;
			auto tagComponent = entity["Tag"];
			if (tagComponent)
				name = tagComponent["tag"].as<std::string>();

			OWL_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name)

			Entity deserializedEntity = scene->createEntityWithUUID(core::UUID{uuid}, name);

			auto transformComponent = entity["Transform"];
			if (transformComponent) {
				// Entities always have transforms
				auto &tc = deserializedEntity.getComponent<component::Transform>();
				tc.translation = transformComponent["translation"].as<glm::vec3>();
				tc.rotation = transformComponent["rotation"].as<glm::vec3>();
				tc.scale = transformComponent["scale"].as<glm::vec3>();
			}

			auto cameraComponent = entity["Camera"];
			if (cameraComponent) {
				auto &cc = deserializedEntity.addComponent<component::Camera>();
				auto cameraProps = cameraComponent["camera"];
				auto projType = magic_enum::enum_cast<SceneCamera::ProjectionType>(cameraProps["projectionType"].as<std::string>());
				if (projType.has_value())
					cc.camera.setProjectionType(projType.value());
				cc.camera.setPerspectiveVerticalFOV(cameraProps["perspectiveFOV"].as<float>());
				cc.camera.setPerspectiveNearClip(cameraProps["perspectiveNear"].as<float>());
				cc.camera.setPerspectiveFarClip(cameraProps["perspectiveFar"].as<float>());

				cc.camera.setOrthographicSize(cameraProps["orthographicSize"].as<float>());
				cc.camera.setOrthographicNearClip(cameraProps["orthographicNear"].as<float>());
				cc.camera.setOrthographicFarClip(cameraProps["orthographicFar"].as<float>());

				cc.primary = cameraComponent["primary"].as<bool>();
				cc.fixedAspectRatio = cameraComponent["fixedAspectRatio"].as<bool>();
			}

			auto spriteRendererComponent = entity["SpriteRenderer"];
			if (spriteRendererComponent) {
				auto &src = deserializedEntity.addComponent<component::SpriteRenderer>();
				src.color = spriteRendererComponent["color"].as<glm::vec4>();
				if (spriteRendererComponent["tilingFactor"])
					src.tilingFactor = spriteRendererComponent["tilingFactor"].as<float>();
				if (spriteRendererComponent["texture"])
					src.texture = renderer::Texture2D::create(spriteRendererComponent["texture"].as<std::string>());
			}

			auto circleRendererComponent = entity["CircleRenderer"];
			if (circleRendererComponent) {
				auto &src = deserializedEntity.addComponent<component::CircleRenderer>();
				src.color = circleRendererComponent["color"].as<glm::vec4>();
				src.thickness = circleRendererComponent["thickness"].as<float>();
				src.fade = circleRendererComponent["fade"].as<float>();
			}
		}
	}

	return true;
}

}// namespace owl::scene
