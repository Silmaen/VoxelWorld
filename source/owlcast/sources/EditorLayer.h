/**
 * @file EditorLayer.h
 * @author Silmaen
 * @date 23/06/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "panels/MapWindow.h"
#include "panels/ViewPort.h"

namespace owl::raycaster {

/**
 * @brief Class EditorLayer.
 */
class EditorLayer final : public core::layer::Layer {
public:
	/**
	 * @brief Default constructor.
	 */
	EditorLayer();
	/**
	 * @brief Default destructor.
	 */
	~EditorLayer() override;
	/**
	 * @brief Default copy constructor.
	 */
	EditorLayer(const EditorLayer &) = delete;
	/**
	 * @brief Default move constructor.
	 */
	EditorLayer(EditorLayer &&) = delete;
	/**
	 * @brief Default copy affectation operator.
	 */
	EditorLayer &operator=(const EditorLayer &) = delete;
	/**
	 * @brief Default move affectation operator.
	 */
	EditorLayer &operator=(EditorLayer &&) = delete;
	void onAttach() override;
	void onDetach() override;
	void onUpdate(const core::Timestep &iTimeStep) override;
	void onEvent(event::Event &ioEvent) override;
	void onImGuiRender(const core::Timestep &iTimeStep) override;

	enum struct Mode { Game, Editor };

private:
	/// The view port window.
	uniq<panel::ViewPort> m_viewPort;
	/// The map window.
	uniq<panel::MapWindow> m_mapWindow;

	Mode currentMode = Mode::Game;

	void renderMenu();

	bool onKeyPressed(event::KeyPressedEvent &ioEvent);
	bool onMouseButtonPressed(event::MouseButtonPressedEvent &ioEvent);
};

}// namespace owl::raycaster
