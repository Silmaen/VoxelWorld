/**
 * @file BaseGauge.cpp
 * @author Silmaen
 * @date 18/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "BaseGauge.h"
#include "renderer/Renderer.h"

namespace drone::panels::gauge {

static const char *texName = "textures/GaugeCover";

BaseGauge::BaseGauge() {
	auto textureLib = owl::renderer::Renderer::getTextureLibrary();
	if (!textureLib.exists(texName))
		textureLib.addFromStandardPath(texName);
	coverTexture = textureLib.get(texName);
};

BaseGauge::~BaseGauge() = default;

void BaseGauge::drawCover() {
	owl::renderer::Renderer2D::drawQuad({.transform = transform,
										 .texture = coverTexture});
}

}// namespace drone::panels::gauge
