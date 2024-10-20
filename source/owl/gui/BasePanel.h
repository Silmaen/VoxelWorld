/**
 * @file BasePanel.h
 * @author Silmaen
 * @date 10/16/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Timestep.h"
#include "event/Event.h"
#include "math/vectors.h"

namespace owl::gui {

/**
 * @brief Class BasePanel.
 */
class OWL_API BasePanel {
public:
	/**
	 * @brief Default constructor.
	 */
	BasePanel();
	/**
	 * @brief Default destructor.
	 */
	virtual ~BasePanel();
	/**
	 * @brief Default copy constructor.
	 */
	BasePanel(const BasePanel&) = default;
	/**
	 * @brief Default move constructor.
	 */
	BasePanel(BasePanel&&) = default;
	/**
	 * @brief Default copy affectation operator.
	 */
	BasePanel& operator=(const BasePanel&) = default;
	/**
	 * @brief Default move affectation operator.
	 */
	BasePanel& operator=(BasePanel&&) = default;
	/**
	 * @brief Initialization when attached.
	 */
	virtual void attach() {}
	/**
	 * @brief Cleaning after detached.
	 */
	virtual void detach() {}
	/**
	 * @brief Update panel Status.
	 * @param iTimeStep The Time delta of the frame.
	 */
	virtual void onUpdate([[maybe_unused]] const core::Timestep& iTimeStep) {}
	/**
	 * @brief Action on event.
	 * @param[in,out] ioEvent The Event to react.
	 */
	virtual void onEvent([[maybe_unused]] event::Event& ioEvent) {}
	/**
	 * @brief Update internal variables and do the rendering.
	 */
	void onRender();

	/**
	 * @brief Do the rendering.
	 */
	virtual void onRenderInternal() = 0;

	/**
	 * @brief get the panel size.
	 */
	const math::vec2ui& getSize() const { return m_size; }

	/**
	 * @brief If the panel is hovered.
	 */
	bool isHovered() const { return m_hovered; }
	/**
	 * @brief If the panel is focused.
	 */
	bool isFocused() const { return m_focused; }

protected:
	/// Panel size.
	math::vec2ui m_size = {0, 0};
	/// Panel bounds.
	math::vec2 m_lower = {0.0f, 0.0f};
	math::vec2 m_upper = {0.0f, 0.0f};
	/// Focused state.
	bool m_focused = false;
	/// Hovered state.
	bool m_hovered = false;
};

}// namespace owl::gui
