/**
 * @file ButtonBar.h
 * @author Silmaen
 * @date 10/26/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Button.h"

namespace owl::gui::widgets {

struct ButtonBarData : WidgetData {
	bool hasDecoration = true;
	bool hasScrollBars = true;
	bool isHorizontal = true;
};
/**
 * @brief Class ButtonBar.
 */
class OWL_API ButtonBar : public Widget<ButtonBarData> {
public:
	/**
	 * @brief Default destructor.
	 */
	~ButtonBar() override;

	ButtonBar() = default;
	ButtonBar(const ButtonBar&) = default;
	ButtonBar(ButtonBar&&) = default;
	auto operator=(const ButtonBar&) -> ButtonBar& = default;
	auto operator=(ButtonBar&&) -> ButtonBar& = default;

	/**
	 * @brief Add a button to render.
	 * @param iButton The button's data to add.
	 */
	void addButton(ButtonData&& iButton);

	/**
	 * @brief Delete all buttons.
	 */
	void clearButtons();

private:
	/**
	 * @brief Render the ButtonBar.
	 */
	void onRenderBase() const override;
	/// List of buttons.
	std::vector<Button> m_buttons;
};

}// namespace owl::gui::widgets
