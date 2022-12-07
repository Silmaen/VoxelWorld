/**
 * @file main.cpp
 * @author Silmaen
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "debug/TrakerClient.h"

#include "owl.h"
#include <imgui.h>

class baseLayer : public owl::core::layer::Layer {
public:
	baseLayer() : Layer("baseLayer") {}
	void onUpdate() override {
		if (owl::input::Input::isKeyPressed(owl::input::key::Tab))
			OWL_INFO("Tab is pressed");
		float *itou = nullptr;
		if (owl::input::Input::isKeyPressed(owl::input::key::M)) {
			itou = new float;
			OWL_TRACE("Toutou");
		}

		if (itou != nullptr)
			delete itou;
	}
	void onEvent([[maybe_unused]] owl::event::Event &event) override {}
	void onImGuiRender() override {
		auto &tracker = owl::debug::Tracker::get();
		ImGui::Begin("Statistics");
		ImGui::Text(fmt::format("Current used memory: {}",
								tracker.globals().allocatedMemory)
							.c_str());
		ImGui::Text(fmt::format("Max used memory: {}", tracker.globals().memoryPeek)
							.c_str());
		ImGui::Text(
				fmt::format("Allocation calls: {}", tracker.globals().allocationCalls)
						.c_str());
		ImGui::Text(fmt::format("Deallocation calls: {}",
								tracker.globals().deallocationCalls)
							.c_str());
		ImGui::End();
	}
};

class Sandbox : public owl::core::Application {
public:
	Sandbox() { pushLayer(owl::mk_shrd<baseLayer>()); }
};

owl::shrd<owl::core::Application> owl::core::createApplication() {
	return owl::mk_shrd<Sandbox>();
}
