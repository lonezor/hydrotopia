/*
 *  Hydrotopia
 *  Copyright (C) 2022 Johan Norberg <lonezor@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>
 */

#pragma once

#include <string>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

enum class ventilation_fan_mode
{
    /** Initializer value */
    none,

    /** Low RPM */
    low,

    /** High RPM */
    high,

    /** Automatic: RPM is automatically selected based on cabinet temperature */
    automatic,
};

//---------------------------------------------------------------------------------------------------------------------

std::string ventilation_fan_mode_str(ventilation_fan_mode mode);

//---------------------------------------------------------------------------------------------------------------------

/** @brief Ventilation fan high RPM upper threshold
 *
 * When cabinet temperature exceeds upper threshold, high RPM is activated.
 */
constexpr double ventilation_fan_high_rpm_upper_threshold = 35;

//---------------------------------------------------------------------------------------------------------------------

/** @brief Ventilation fan high RPM lower threshold
 *
 * When running at high RPM and cabinet temperature drops below lower threshold,
 * low RPM is activated.
 *
 * Using upper/lower thresholds avoids frequent mode transitions.
 */
constexpr double ventilation_fan_high_rpm_lower_threshold = 30;

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
