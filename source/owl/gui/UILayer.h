/**
 * @file UILayer.h
 * @author Silmaen
 * @date 05/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Theme.h"
#include "core/layer/Layer.h"

/**
 * @brief Namespace for gui
 */
namespace owl::gui {

/**
 * @brief Class ImGuiLayer
 */
class OWL_API UILayer : public core::layer::Layer {
public:
	UILayer(const UILayer &) = delete;
	UILayer(UILayer &&) = delete;
	UILayer &operator=(const UILayer &) = delete;
	UILayer &operator=(UILayer &&) = delete;

	/**
	 * @brief Default constructor.
	 */
	UILayer();

	/**
	 * @brief Destructor.
	 */
	~UILayer() override;

	/**
	 * @brief Action on Attach.
	 */
	void onAttach() override;

	/**
	 * @brief Action on detach.
	 */
	void onDetach() override;

	/**
	 * @brief Action on event.
	 * @param event The Event to react.
	 */
	void onEvent([[maybe_unused]] event::Event &event) override;

	/**
	 * @brief Begin layer definition.
	 */
	void begin();

	/**
	 * @brief End layer definition.
	 */
	void end();

	/**
	 * @brief Defines layer as blocking events.
	 * @param block If layer block event or let them pass to next layer.
	 */
	void blockEvents(bool block) { blockEvent = block; }

	/**
	 * @brief Defines the theme for the UI.
	 * @param theme The theme to apply.
	 */
	void setTheme(const Theme &theme = Theme());

	/**
	 * @brief Enable docking of windows.
	 */
	void enableDocking() { dockingEnable = true; }

	/**
	 * @brief Disable docking of windows.
	 */
	void disableDocking() { dockingEnable = false; }

private:
	/// If event should be bocked.
	bool blockEvent = true;
	/// If the docking space exists.
	bool dockingEnable = false;
	/// Function that initialize the docking port.
	void initializeDocking();
};

}// namespace owl::gui
