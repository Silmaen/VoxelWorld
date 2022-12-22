/**
 * @file SceneHierarchy.cpp
 * @author Silmaen
 * @date 26/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "SceneHierarchy.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "scene/SceneCamera.h"
#include "scene/component/Camera.h"
#include "scene/component/SpriteRenderer.h"
#include "scene/component/Tag.h"
#include "scene/component/Transform.h"

namespace owl::panel {

SceneHierarchy::SceneHierarchy(const shrd<scene::Scene> &context_) {
	setContext(context_);
}

void SceneHierarchy::setContext(const shrd<scene::Scene> &context_) {
	context = context_;
}

void SceneHierarchy::onImGuiRender() {
	ImGui::Begin("Scene Hierarchy");

	context->registry.each([&](auto entityID) {
		scene::Entity entity{entityID, context.get()};
		drawEntityNode(entity);
	});

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		selection = {};

	ImGui::End();

	ImGui::Begin("Properties");
	if (selection)
		drawComponents(selection);

	ImGui::End();
}

void SceneHierarchy::drawEntityNode(scene::Entity entity) {
	auto &tag = entity.getComponent<scene::component::Tag>().tag;

	ImGuiTreeNodeFlags flags = ((selection == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx((void *) (uint64_t) (uint32_t) entity, flags, tag.c_str());
	if (ImGui::IsItemClicked()) {
		selection = entity;
	}


	if (opened) {
		ImGuiTreeNodeFlags flags_o = ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened_o = ImGui::TreeNodeEx((void *) 9817239, flags_o, tag.c_str());
		if (opened_o)
			ImGui::TreePop();
		ImGui::TreePop();
	}
}

void SceneHierarchy::drawComponents(scene::Entity entity) {
	if (entity.hasComponent<scene::component::Tag>()) {
		auto &tag = entity.getComponent<scene::component::Tag>().tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), tag.c_str());
		if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
			tag = std::string(buffer);
		}
	}

	if (entity.hasComponent<scene::component::Transform>()) {
		if (ImGui::TreeNodeEx((void *) typeid(scene::component::Transform).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
			auto &transform = entity.getComponent<scene::component::Transform>().transform;
			ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

			ImGui::TreePop();
		}
	}

	if (entity.hasComponent<scene::component::Camera>()) {
		if (ImGui::TreeNodeEx((void *) typeid(scene::component::Camera).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
			auto &cameraComponent = entity.getComponent<scene::component::Camera>();
			auto &camera = cameraComponent.camera;

			ImGui::Checkbox("Primary", &cameraComponent.primary);

			const char *projectionTypeStrings[] = {"Perspective", "Orthographic"};
			const char *currentProjectionTypeString = projectionTypeStrings[(int) camera.getProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.setProjectionType((scene::SceneCamera::ProjectionType) i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.getProjectionType() == scene::SceneCamera::ProjectionType::Perspective) {
				float perspectiveVerticalFov = glm::degrees(camera.getPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
					camera.setPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

				float perspectiveNear = camera.getPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera.setPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.getPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera.setPerspectiveFarClip(perspectiveFar);
			}

			if (camera.getProjectionType() == scene::SceneCamera::ProjectionType::Orthographic) {
				float orthoSize = camera.getOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.setOrthographicSize(orthoSize);

				float orthoNear = camera.getOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.setOrthographicNearClip(orthoNear);

				float orthoFar = camera.getOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.setOrthographicFarClip(orthoFar);

				ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);
			}
			ImGui::TreePop();
		}
	}
	if (entity.hasComponent<scene::component::SpriteRenderer>()) {
		if (ImGui::TreeNodeEx((void *) typeid(scene::component::SpriteRenderer).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer")) {
			auto &src = entity.getComponent<scene::component::SpriteRenderer>();
			ImGui::ColorEdit4("Color", glm::value_ptr(src.color));
			ImGui::TreePop();
		}
	}
}

}// namespace owl::panel
