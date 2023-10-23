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
class CameraSystem {
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
	 * @brief Get the number of camera.
	 * @param recompute Recompute the number of camera.
	 * @return Get the number of camera.
	 */
	[[nodiscard]] size_t getNbCamera(bool recompute = false);

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
	[[nodiscard]] int32_t getCurrentCamera() const;

	/**
	 * @brief Actualize the list of available cameras.
	 */
	void actualiseList();

	/**
	 * @brief object for camera list.
	 */
	struct CameraInfo {
		/// The camera port.
		std::string port{};
		/// Camera Id.
		int32_t id = -1;
		/// Device Name.
		std::string name{};
		/// Bus information.
		std::string busInfo{};
	};

	using CameraList = std::vector<CameraInfo>;

	/**
	 * @brief Gat the list of camera.
	 * @return Lit of Camera.
	 */
	[[nodiscard]] const CameraList &getListOfCamera() const { return cameraList; }

private:
	/**
	 * @brief Constructor.
	 */
	CameraSystem();

	void resize(uint32_t nw, uint32_t nh);

	uint32_t width = 0;
	uint32_t height = 0;

	int32_t frameSkip = 0;
	int32_t frameCheck = 50;
	int32_t frameCount = 0;

	owl::shared<owl::renderer::Texture> frame;
	CameraList cameraList;
};

}// namespace drone::IO
