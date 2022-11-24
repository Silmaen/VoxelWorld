/**
 * @file Angle.h
 * @author Silmaen
 * @date 02/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

/**
 * @brief Namespace for mathematical objects
 */
namespace vw::geom {

/**
 * @brief Class Angle
 */
class Angle {
public:
    /**
     * @brief Default copy constructor
     */
    Angle(const Angle&) = default;
    /**
     * @brief Default move constructor
     */
    Angle(Angle&&) = default;
    /**
     * @brief Default copy assignation
     * @return this
     */
    Angle& operator=(const Angle&) = default;
    /**
     * @brief Default move assignation
     * @return this
     */
    Angle& operator=(Angle&&) = default;
    /**
     * @brief Default constructor.
     */
    Angle() = default;
    /**
     * @brief Destructor.
     */
    ~Angle() = default;//---UNCOVER---
    /**
     * @brief List of angle's units
     */
    enum struct Unit {
        Degree,///< Degree
        Radian ///< Radian
    };
    /**
     * @brief Constructor by value and unit
     * @param ang The value
     * @param unit The unit
     */
    Angle(double ang, const Unit& unit);
    /**
     * @brief redefine the value with unit
     * @param ang The value
     * @param unit The unit
     */
    void set(double ang, const Unit& unit = Unit::Degree);

    /**
     * @brief Clamp the value from 0 to one turn
     */
    void clamp();
    /**
     * @brief Clamp the value from minus half turn to half turn
     */
    void clampSigned();

    /**
     * @brief Direct access to value
     * @return The value in Radian
     */
    [[nodiscard]] const double& get() const { return Value; }
    /**
     * @brief Direct access to value
     * @return The value in Radian
     */
    [[nodiscard]] double& get() { return Value; }
    /**
     * @brief Get the value in the desired unit
     * @param unit Unit required
     * @return The value
     */
    [[nodiscard]] double getUnit(const Unit& unit = Unit::Degree) const {
        if (unit == Unit::Degree)
            return RadToDeg(Value);
        return Value;
    }
    /**
     * @brief Access to the value of Pi
     * @return Pi
     */
    static double getPi();
private:
    static double RadToDeg(double value);
    static double DegToRad(double value);
    /// Value of the angle, always stored in radian
    double Value = 0;
};

}// namespace rc::math
