/**
 * @file MapWindow.h
 * @author Silmaen
 * @date 23/06/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <owl.h>

namespace owl::raycaster::panel {

/**
 * @brief Class MapWindow.
 */
class MapWindow final : public gui::BaseDrawPanel {
public:
	/**
	 * @brief Default constructor.
	 */
	MapWindow();
	/**
	 * @brief Default destructor.
	 */
	~MapWindow() override;
	/**
	 * @brief Default copy constructor.
	 */
	MapWindow(const MapWindow &) = default;
	/**
	 * @brief Default move constructor.
	 */
	MapWindow(MapWindow &&) = default;
	/**
	 * @brief Default copy affectation operator.
	 */
	MapWindow &operator=(const MapWindow &) = default;
	/**
	 * @brief Default move affectation operator.
	 */
	MapWindow &operator=(MapWindow &&) = default;
	/**
	 * @brief Update panel Status.
	 * @param iTimeStep The Time delta of the frame.
	 */
	void onUpdate(const core::Timestep &iTimeStep) override;

private:
};

}// namespace owl::raycaster::panel
