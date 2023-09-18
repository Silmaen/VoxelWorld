/**
 * @file droneLayer.h
 * @author Silmaen
 * @date 16/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "event/KeyEvent.h"
#include "owl.h"

namespace drone {

class droneLayer : public owl::core::layer::Layer {
public:
	droneLayer(const droneLayer &) = delete;
	droneLayer(droneLayer &&) = delete;
	droneLayer &operator=(const droneLayer &) = delete;
	droneLayer &operator=(droneLayer &&) = delete;
	/**
	 * @brief Default constructor.
	 */
	droneLayer();
	/**
	 * @brief Destructor.
	 */
	~droneLayer() override = default;

	void onAttach() override;
	void onDetach() override;
	void onUpdate(const owl::core::Timestep &ts) override;
	void onEvent(owl::event::Event &event) override;
	void onImGuiRender(const owl::core::Timestep &ts) override;

private:
	void renderStats(const owl::core::Timestep &ts);
	void renderMenu();
	void renderToolbar();

	// to be placed in separate classes
	void renderSettings();
	void renderViewport();
	void renderGauges();
	void renderMisc();

	bool onKeyPressed(owl::event::KeyPressedEvent &e);
	bool onMouseButtonPressed(owl::event::MouseButtonPressedEvent &e);

	bool isConnected() const;
	void toggleConnect();

	bool viewportFocused = false;
	bool viewportHovered = false;
	glm::vec2 viewportSize = {0.0f, 0.0f};
	glm::vec2 viewportBounds[2] = {{0.0f, 0.0f}, {0.0f, 0.0f}};
	owl::shared<owl::renderer::Framebuffer> framebuffer;

	bool connected = false;
	bool showStats = true;

	enum struct DisplayMode {
		Settings,
		Gauges
	};
	DisplayMode mode = DisplayMode::Gauges;

	// Icon resources
	owl::shared<owl::renderer::Texture2D> iconExit;
	owl::shared<owl::renderer::Texture2D> iconGauges;
	owl::shared<owl::renderer::Texture2D> iconSettings;
	owl::shared<owl::renderer::Texture2D> iconConnected;
	owl::shared<owl::renderer::Texture2D> iconConnect;
};

}// namespace drone
