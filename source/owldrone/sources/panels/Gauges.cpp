/**
 * @file Gauges.cpp
 * @author Silmaen
 * @date 18/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "Gauges.h"

#include "gauge/AirSpeed.h"
#include "gauge/Altitude.h"
#include "gauge/ArtificialHorizon.h"
#include "gauge/Compas.h"
#include "gauge/MotorRate.h"
#include "gauge/VerticalSpeed.h"

using namespace owl;

namespace drone::panels {

Gauges::Gauges() {

	renderer::FramebufferSpecification specs;
	specs.attachments = {
			renderer::FramebufferTextureFormat::Rgba8,
			renderer::FramebufferTextureFormat::RedInteger,
			renderer::FramebufferTextureFormat::Depth};
	specs.width = 1280;
	specs.height = 720;
	framebuffer = renderer::Framebuffer::create(specs);

	// camera
	camera = mkShared<renderer::CameraOrtho>(0, 1280, 0, 720);

	// gauge for speed
	gauges.emplace_back(mkShared<gauge::AirSpeed>());
	// gauge for vertical speed
	gauges.emplace_back(mkShared<gauge::VerticalSpeed>());
	// gauge for altitude
	gauges.emplace_back(mkShared<gauge::Altitude>());
	// gauge for compass
	gauges.emplace_back(mkShared<gauge::Compas>());
	// gauge for motor rates
	gauges.emplace_back(mkShared<gauge::MotorRate>());
	// gauge for artificial horizon
	gauges.emplace_back(mkShared<gauge::ArtificialHorizon>());
}

Gauges::~Gauges() = default;

void Gauges::onUpdate(const core::Timestep &) {
	OWL_PROFILE_FUNCTION()

	// Updatte data from flight controller
	auto &rc = getRemoteController();
	if (rc) {
		static_pointer_cast<gauge::AirSpeed>(gauges[0])->setVelocity(rc->getHorizontalVelocity());
		static_pointer_cast<gauge::VerticalSpeed>(gauges[1])->setVerticalVelocity(rc->getVerticalVelocity());
		static_pointer_cast<gauge::Altitude>(gauges[2])->setAltitude(rc->getAltitude());
		const auto angles = rc->getRotations();
		static_pointer_cast<gauge::Compas>(gauges[3])->setHeading(angles.z);
		static_pointer_cast<gauge::MotorRate>(gauges[4])->setMotorRates(rc->getMotorRates());
		static_pointer_cast<gauge::ArtificialHorizon>(gauges[5])->setPitchRoll(angles.y, angles.x);
	}

	// Draw into the frame buffer
	const auto spec = framebuffer->getSpecification();
	const auto width = static_cast<uint32_t>(viewportSize.x);
	const auto height = static_cast<uint32_t>(viewportSize.y);
	const float aspectRatio = viewportSize.x / viewportSize.y;
	float scaling = std::min(aspectRatio, 0.666f);

	if (width > 0 && height > 0 && (width != spec.width || height != spec.height)) {
		framebuffer->resize(width, height);
		viewportSize = glm::vec2(width, height);
		camera->setProjection(-aspectRatio, aspectRatio, -1, 1);
	}
	// Render
	framebuffer->bind();
	renderer::RenderCommand::setClearColor({0.0f, 0.0f, 0.0f, 1});
	renderer::RenderCommand::clear();

	// Clear our entity ID attachment to -1
	framebuffer->clearAttachment(1, -1);

	// defines pos an scale
	glm::vec3 pos = {-0.5f * scaling, 1.f * scaling, 0};
	for (const auto &gauge: gauges) {
		gauge->setScale({scaling, scaling});
		gauge->setPosition(pos);
		pos.x *= -1;
		if (pos.x < 0)
			pos.y -= scaling;
	}
	// Do the drawings!
	// ===============================================================
	renderer::Renderer2D::beginScene(*camera);

	// draw all backgrounds
	for (const auto &gauge: gauges) { gauge->drawBack(); }
	// draw all cursors

	for (const auto &gauge: gauges) { gauge->drawCursors(); }
	// draw all covers
	for (const auto &gauge: gauges) { gauge->drawCover(); }

	renderer::Renderer2D::endScene();
	// ===============================================================
	// free the frame buffer.
	framebuffer->unbind();
}

void Gauges::onRender() {
	OWL_PROFILE_FUNCTION()

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Drone Gauges");
	const auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	const auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	const auto viewportOffset = ImGui::GetWindowPos();
	viewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
	viewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

	viewportFocused = ImGui::IsWindowFocused();
	viewportHovered = ImGui::IsWindowHovered();
	core::Application::get().getImGuiLayer()->blockEvents(!viewportFocused && !viewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
	if (const uint64_t textureID = framebuffer->getColorAttachmentRendererId(0); textureID != 0)
		ImGui::Image(reinterpret_cast<void *>(textureID), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});
	else
		OWL_WARN("No frameBuffer to render...")

	ImGui::End();
	ImGui::PopStyleVar();
}

}// namespace drone::panels
