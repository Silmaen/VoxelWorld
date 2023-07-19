/**
 * @file trigonometry.h
 * @author Silmaen
 * @date 02/01/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

namespace owl::math {

/**
 * @brief Arc tangent 2, safe atan(y/(x + |x,y|)).
 * @param y Numerator.
 * @param x Denominator.
 * @return Arc tangent with the right quadrant.
 */
OWL_API float atan2(float y, float x);

}// namespace owl::math
