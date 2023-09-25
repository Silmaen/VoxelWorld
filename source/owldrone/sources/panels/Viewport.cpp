/**
 * @file Viewport.cpp
 * @author Silmaen
 * @date 18/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "Viewport.h"

#include <imgui.h>

using namespace owl;

namespace drone::panels {

Viewport::Viewport() {
	renderer::FramebufferSpecification specs;
	specs.attachments = {
			renderer::FramebufferTextureFormat::RGBA8,
			renderer::FramebufferTextureFormat::RED_INTEGER,
			renderer::FramebufferTextureFormat::Depth};
	specs.width = 1280;
	specs.height = 720;
	framebuffer = renderer::Framebuffer::create(specs);
}

Viewport::~Viewport() = default;

void Viewport::onUpdate(const owl::core::Timestep &) {
	OWL_PROFILE_FUNCTION()

	auto spec = framebuffer->getSpecification();
	auto width = static_cast<uint32_t>(viewportSize.x);
	auto height = static_cast<uint32_t>(viewportSize.y);
	if (width > 0 && height > 0 && (width != spec.width || height != spec.height)) {
		framebuffer->resize(width, height);
		viewportSize = glm::vec2(width, height);
	}
	// Render
	framebuffer->bind();
	renderer::RenderCommand::setClearColor({0.2f, 0.2f, 0.2f, 1});
	renderer::RenderCommand::clear();

	// Clear our entity ID attachment to -1
	framebuffer->clearAttachment(1, -1);

	// Do the drawings!
	// ===============================================================


	// ===============================================================
	// free the frame buffer.
	framebuffer->unbind();
}

void Viewport::onRender() {
	OWL_PROFILE_FUNCTION()

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Drone Viewport");
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	viewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
	viewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

	viewportFocused = ImGui::IsWindowFocused();
	viewportHovered = ImGui::IsWindowHovered();
	core::Application::get().getImGuiLayer()->blockEvents(!viewportFocused && !viewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
	uint64_t textureID = framebuffer->getColorAttachmentRendererID();
	if (textureID != 0)
		ImGui::Image(reinterpret_cast<void *>(textureID), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
			const char *path = reinterpret_cast<const char *>(payload->Data);
			std::filesystem::path scenePath = core::Application::get().getAssetDirectory() / path;
			OWL_CORE_WARN("Could not load file {}: unsupported format.", scenePath.string())
		}
		ImGui::EndDragDropTarget();
	}


	ImGui::End();
	ImGui::PopStyleVar();
}


}// namespace drone::panels
