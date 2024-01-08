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
#include <imgui_internal.h>
#include <magic_enum.hpp>

#include "core/Application.h"
#include "scene/SceneCamera.h"
#include "scene/component/Camera.h"
#include "scene/component/CircleRenderer.h"
#include "scene/component/SpriteRenderer.h"
#include "scene/component/Tag.h"
#include "scene/component/Transform.h"

namespace owl::panel {

[[maybe_unused]] SceneHierarchy::SceneHierarchy(const shared<scene::Scene> &context_) {
	setContext(context_);
}

void SceneHierarchy::setContext(const shared<scene::Scene> &context_) {
	context = context_;
	selection = {};
}

void SceneHierarchy::onImGuiRender() {
	ImGui::Begin("Scene Hierarchy");

	if (context) {
		context->registry.each([&](auto entityID) {
			scene::Entity entity{entityID, context.get()};
			drawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			selection = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(nullptr, 1)) {
			if (ImGui::MenuItem("Create Empty Entity"))
				context->createEntity("Empty Entity");
			ImGui::EndPopup();
		}
	}
	ImGui::End();

	ImGui::Begin("Properties");
	if (selection)
		drawComponents(selection);

	ImGui::End();
}

void SceneHierarchy::drawEntityNode(scene::Entity entity) {
	auto &tag = entity.getComponent<scene::component::Tag>().tag;

	ImGuiTreeNodeFlags flags = ((selection == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx(reinterpret_cast<void *>(static_cast<uint64_t>(static_cast<uint32_t>(entity))), flags, "%s", tag.c_str());
	if (ImGui::IsItemClicked()) {
		selection = entity;
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;
		ImGui::EndPopup();
	}

	if (opened) {
		ImGuiTreeNodeFlags flags_o = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened_o = ImGui::TreeNodeEx(reinterpret_cast<void *>(9817239), flags_o, "%s", tag.c_str());
		if (opened_o)
			ImGui::TreePop();
		ImGui::TreePop();
	}

	if (entityDeleted) {
		context->destroyEntity(entity);
		if (selection == entity)
			selection = {};
	}
}

static void drawVec3Control(const std::string &label, glm::vec3 &values, float resetValue = 0.0f, float columnWidth = 100.0f) {
	const ImGuiIO &io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

template<typename T, typename UIFunction>
static void drawComponent(const std::string &name, scene::Entity entity, UIFunction uiFunction) {
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (entity.hasComponent<T>()) {
		auto &component = entity.getComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx(reinterpret_cast<void *>(typeid(T).hash_code()), treeNodeFlags, "%s", name.c_str());
		ImGui::PopStyleVar();
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{lineHeight, lineHeight})) {
			ImGui::OpenPopup("ComponentSettings");
		}
		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings")) {
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;
			ImGui::EndPopup();
		}
		if (open) {
			uiFunction(component);
			ImGui::TreePop();
		}
		if (removeComponent)
			entity.removeComponent<T>();
	}
}

void SceneHierarchy::drawComponents(scene::Entity entity) {
	if (entity.hasComponent<scene::component::Tag>()) {
		auto &tag = entity.getComponent<scene::component::Tag>().tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		memcpy(buffer, tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
			tag = std::string(buffer);
		}
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);
	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");
	if (ImGui::BeginPopup("AddComponent")) {
		if (!selection.hasComponent<scene::component::Camera>()) {
			if (ImGui::MenuItem("Camera")) {
				selection.addComponent<scene::component::Camera>();
				ImGui::CloseCurrentPopup();
			}
		}
		if (!selection.hasComponent<scene::component::SpriteRenderer>()) {
			if (ImGui::MenuItem("Sprite Renderer")) {
				selection.addComponent<scene::component::SpriteRenderer>();
				ImGui::CloseCurrentPopup();
			}
		}
		if (!selection.hasComponent<scene::component::CircleRenderer>()) {
			if (ImGui::MenuItem("Circle Renderer")) {
				selection.addComponent<scene::component::CircleRenderer>();
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::EndPopup();
	}
	ImGui::PopItemWidth();

	drawComponent<scene::component::Transform>("Transform", entity, [](auto &component) {
		drawVec3Control("Translation", component.translation);
		glm::vec3 rotation = glm::degrees(component.rotation);
		drawVec3Control("Rotation", rotation);
		component.rotation = glm::radians(rotation);
		drawVec3Control("Scale", component.scale, 1.0f);
	});
	drawComponent<scene::component::Camera>("Camera", entity, [](auto &component) {
		auto &camera = component.camera;
		ImGui::Checkbox("Primary", &component.primary);
		const char *projectionTypeStrings[] = {"Perspective", "Orthographic"};
		const char *currentProjectionTypeString = projectionTypeStrings[static_cast<int>(camera.getProjectionType())];
		if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
			for (int i = 0; i < 2; i++) {
				bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
				if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
					currentProjectionTypeString = projectionTypeStrings[i];
					camera.setProjectionType(static_cast<scene::SceneCamera::ProjectionType>(i));
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
			ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
		}
	});
	drawComponent<scene::component::SpriteRenderer>("Sprite Renderer", entity, [](auto &component) {
		ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const char *path = reinterpret_cast<const char *>(payload->Data);
				std::filesystem::path texturePath = core::Application::get().getAssetDirectory() / path;
				component.texture = renderer::Texture2D::create(texturePath);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::DragFloat("Tiling Factor", &component.tilingFactor, 0.1f, 0.0f, 100.0f);
	});
	drawComponent<scene::component::CircleRenderer>("Circle Renderer", entity, [](auto &component) {
		ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		ImGui::DragFloat("Thickness", &component.thickness, 0.025f, 0.0f, 1.0f);
		ImGui::DragFloat("Fade", &component.fade, 0.00025f, 0.0f, 1.0f);
	});
}

}// namespace owl::panel
