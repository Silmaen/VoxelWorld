/**
 * @file MapWindow.cpp
 * @author Silmaen
 * @date 23/06/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "MapWindow.h"

namespace owl::raycaster::panel {

MapWindow::MapWindow() {
	const renderer::FramebufferSpecification specs{
			.size = {1280, 720},
			.attachments = {{renderer::AttachmentSpecification::Format::Surface,
							 renderer::AttachmentSpecification::Tiling::Optimal},
							{renderer::AttachmentSpecification::Format::RedInteger,
							 renderer::AttachmentSpecification::Tiling::Optimal}},
			.samples = 1,
			.swapChainTarget = false,
			.debugName = "mapWindow"};
	mp_framebuffer = renderer::Framebuffer::create(specs);
	// camera
	m_camera = mkShared<renderer::CameraOrtho>(0, 1280, 0, 720);
	setName("mapWindow");
}

MapWindow::~MapWindow() = default;

void MapWindow::onUpdate([[maybe_unused]] const core::Timestep &iTimeStep) { OWL_PROFILE_FUNCTION() }

}// namespace owl::raycaster::panel
