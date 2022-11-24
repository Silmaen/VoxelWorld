/**
 * @file Angle.cpp
 * @author Silmaen
 * @date 02/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "Angle.h"
#include <cmath>

constexpr double Pi  = M_PI; ///< Pi
constexpr double Pi2 = Pi * 2.0; ///< 2 pi
constexpr double r2d = 180 / Pi; ///< constant to convert radian to degree
constexpr double d2r = Pi / 180.0; ///< constant to convert degree to radian

namespace vw::geom {

Angle::Angle(double ang, const Angle::Unit& unit) {
    set(ang, unit);
}

void Angle::set(double ang, const Angle::Unit& unit) {
    if (unit == Unit::Degree) {
        Value = DegToRad(ang);
    } else {
        Value = ang;
    }
}
void Angle::clamp() {
    while (Value < 0) {
        Value += Pi2;
    }
    while (Value >= Pi2) {
        Value -= Pi2;
    }
}

void Angle::clampSigned() {
    while (Value <= -Pi) {
        Value += Pi2;
    }
    while (Value > Pi) {
        Value -= Pi2;
    }
}

double Angle::RadToDeg(const double value) {
    return value * r2d;
}

double Angle::DegToRad(const double value) {
    return value * d2r;
}

double Angle::getPi() {
    return Pi;
}

}// namespace rc::math
