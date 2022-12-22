/**
 * @file ImGuiLayer.h
 * @author Silmaen
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/layer/Layer.h"

/**
 * @brief Namespace for gui
 */
namespace owl::gui {

/**
 * @brief Class ImGuiLayer
 */
class OWL_API ImGuiLayer : public core::layer::Layer {
public:
	ImGuiLayer(const ImGuiLayer &) = delete;
	ImGuiLayer(ImGuiLayer &&) = delete;
	ImGuiLayer &operator=(const ImGuiLayer &) = delete;
	ImGuiLayer &operator=(ImGuiLayer &&) = delete;
	/**
	 * @brief Default constructor.
	 */
	ImGuiLayer();
	/**
	 * @brief Destructor.
	 */
	~ImGuiLayer() override;
	/**
	 * @brief Action on Attach
	 */
	void onAttach() override;
	/**
	 * @brief Action on detach
	 */
	void onDetach() override;
	/**
	 * @brief Action on event
	 * @param event The Event to react
	 */
	void onEvent([[maybe_unused]] event::Event &event) override;

	void begin();

	void end();

	void blockEvents(bool block) { blockEvent = block; }

	void setDarkThemeColors();

	void enableDocking(){ dockingEnable =true; }

	void disableDocking(){ dockingEnable =false; }

private:
	/// If event should be bocked
	bool blockEvent = true;
	/// If the docking space exists
	bool dockingEnable = false;
	/// Function that initialize the docking port
	void initializeDocking();
};

} // namespace owl::gui
