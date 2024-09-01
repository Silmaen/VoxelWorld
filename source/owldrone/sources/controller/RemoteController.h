/**
 * @file RemoteController.h
 * @author Silmaen
 * @date 21/09/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include <owl.h>

namespace drone::controller {

/**
 * @brief Class RemoteController
 */
class RemoteController final {
public:
	/**
	 * @brief Constructor.
	 * @param motorNumber The number of motor in the device.
	 */
	explicit RemoteController(size_t motorNumber = 4);

	/**
	 * @brief Destructor.
	 */
	~RemoteController();

	/**
	 * @brief Copy constructor.
	 */
	RemoteController(const RemoteController&) = default;

	/**
	 * @brief Move constructor.
	 */
	RemoteController(RemoteController&&) = default;

	/**
	 * @brief Copy affectation operator.
	 */
	auto operator=(const RemoteController&) -> RemoteController& = default;

	/**
	 * @brief Move affectation operator.
	 */
	auto operator=(RemoteController&&) -> RemoteController& = default;


	/**
	 * @brief Reads the horizontal velocity.
	 * @return The horizontal velocity.
	 */
	[[nodiscard]] auto getHorizontalVelocity() const -> const float& { return m_horizontalVelocity; }

	/**
	 * @brief Reads the vertical velocity.
	 * @return The vertical velocity.
	 */
	[[nodiscard]] auto getVerticalVelocity() const -> const float& { return m_verticalVelocity; }

	/**
	 * @brief Reads the m_altitude.
	 * @return The m_altitude.
	 */
	[[nodiscard]] auto getAltitude() const -> const float& { return m_altitude; }

	/**
	 * @brief Reads the rotation vector.
	 * @return The rotation vector.
	 */
	[[nodiscard]] auto getRotations() const -> const owl::math::vec3& { return m_rotations; }

	/**
	 * @brief Reads the motor rates.
	 * @return The motor rates.
	 */
	[[nodiscard]] auto getMotorRates() const -> const std::vector<float>& { return m_motors; }

	/**
	 * @brief Defines the horizontal velocity.
	 * @param velocity The horizontal velocity.
	 */
	void setHorizontalVelocity(float velocity) { m_horizontalVelocity = velocity; }

	/**
	 * @brief Defines the vertical velocity.
	 * @param velocity The vertical velocity.
	 */
	void setVerticalVelocity(float velocity) { m_verticalVelocity = velocity; }

	/**
	 * @brief Defines the current m_altitude.
	 * @param alt The m_altitude.
	 */
	void setAltitude(float alt) { m_altitude = alt; }

	/**
	 * @brief Defines the rotation vector.
	 * @param rot The rotation vector.
	 */
	void setRotation(const owl::math::vec3& rot) { m_rotations = rot; }

	/**
	 * @brief Defines the motor rates.
	 * @param mot The motor rates.
	 */
	void setMotorRates(const std::vector<float>& mot);

private:
	/// The horizontal velocity.
	float m_horizontalVelocity = 0;
	/// The vertical velocity.
	float m_verticalVelocity = 0;
	/// The current m_altitude.
	float m_altitude = 0;
	/// The rotation vector roll pitch yaw.
	owl::math::vec3 m_rotations = {0.f, 0.f, 0.f};
	/// The vertical velocity.
	std::vector<float> m_motors;
};

}// namespace drone::controller
