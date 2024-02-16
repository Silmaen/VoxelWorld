/**
 * @file CameraSystem.h
 * @author Silmaen
 * @date 28/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include <owl.h>

namespace drone::IO {

/**
 * @brief Class CameraSystem
 */
class CameraSystem final {
public:
	/**
	 * @brief Destructor.
	 */
	virtual ~CameraSystem();

	CameraSystem(const CameraSystem &) = delete;
	CameraSystem(CameraSystem &&) = delete;
	CameraSystem &operator=(const CameraSystem &) = delete;
	CameraSystem &operator=(CameraSystem &&) = delete;

	/**
	 * @brief Singleton get.
	 * @return Instance of the Camera System.
	 */
	static CameraSystem &get() {
		static CameraSystem instance;
		return instance;
	}

	/**
	 * @brief Frame update of the camera
	 */
	void onUpdate(const owl::core::Timestep &ts);

	/**
	 * @brief Get the camera frame.
	 * @return The camera frame.
	 */
	[[nodiscard]] const owl::shared<owl::renderer::Texture> &getFrame() const { return frame; }

	/**
	 * @brief Set the camera by its ID.
	 * @param id Id of the camera.
	 *
	 * @note If the Camera does not match an id of the list, id will be set to 0.
	 */
	void setCamera(int32_t id);

	/**
	 * @brief Get the current Camera Id.
	 * @return The current camera ID.
	 */
	[[nodiscard]] int32_t getCurrentCameraId() const;
	/**
	 * @brief Get the current Camera Id.
	 * @return The current camera ID.
	 */
	[[nodiscard]] std::string getCurrentCameraName() const;

	/**
	 * @brief Actualize the list of available cameras.
	 */
	void actualiseList();

	/**
	 * @brief Gat the list of camera.
	 * @return Lit of Camera.
	 */
	[[nodiscard]] std::vector<std::string> getListOfCameraNames() const;

	/**
	 * @brief Destroy internal objects.
	 */
	void invalidate();

private:
	/**
	 * @brief Constructor.
	 */
	CameraSystem();

	void resize(const owl::math::FrameSize &size);

	owl::math::FrameSize size;

	int32_t frameSkip = 0;
	int32_t frameCheck = 50;
	int32_t frameCount = 0;

	owl::shared<owl::renderer::Texture> frame;
};

}// namespace drone::IO
