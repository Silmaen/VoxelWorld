/**
 * @file MotorRate.cpp
 * @author Silmaen
 * @date 24/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "MotorRate.h"

namespace drone::panels::gauge {

constexpr float maxRate = 6000;
constexpr float maxAngle = 67.5;

MotorRate::MotorRate() {
	auto textureLib = owl::renderer::Renderer::getTextureLibrary();
	textureLib.addFromStandardPath("textures/GaugeMOTBack");
	textureLib.addFromStandardPath("textures/GaugeMOTCursor1");
	textureLib.addFromStandardPath("textures/GaugeMOTCursor2");
	textureLib.addFromStandardPath("textures/GaugeMOTCursor3");
	textureLib.addFromStandardPath("textures/GaugeMOTCursor4");
	background = textureLib.get("textures/GaugeMOTBack");
	cursor1 = textureLib.get("textures/GaugeMOTCursor1");
	cursor2 = textureLib.get("textures/GaugeMOTCursor2");
	cursor3 = textureLib.get("textures/GaugeMOTCursor3");
	cursor4 = textureLib.get("textures/GaugeMOTCursor4");
}

MotorRate::~MotorRate() = default;

void MotorRate::drawBack() {
	owl::math::Transform tran{getPosition(), {0, 0, 0}, getScale()};
	tran.translation().z() = -0.1f;
	owl::renderer::Renderer2D::drawQuad({.transform = tran, .texture = background});
}

void MotorRate::drawCursors() {
	owl::math::Transform tran{getPosition(), {0, 0, -rateToAngle(0)}, getScale()};
	tran.translation().z() = -0.08f;
	owl::renderer::Renderer2D::drawQuad({.transform = tran, .texture = cursor1});
	tran.rotation().z() = rateToAngle(1);
	tran.translation().z() = -0.07f;
	owl::renderer::Renderer2D::drawQuad({.transform = tran, .texture = cursor2});
	tran.rotation().z() = rateToAngle(2);
	tran.translation().z() = -0.06f;
	owl::renderer::Renderer2D::drawQuad({.transform = tran, .texture = cursor3});
	tran.rotation().z() = -rateToAngle(3);
	tran.translation().z() = -0.05f;
	owl::renderer::Renderer2D::drawQuad({.transform = tran, .texture = cursor4});
}

auto MotorRate::rateToAngle(uint8_t idx) const -> float {
	return -std::clamp(rates[idx], 0.f, maxRate) / maxRate * maxAngle;
}


}// namespace drone::panels::gauge
