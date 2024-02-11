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

#include <memory>
#include <vector>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

// Relay idx 0-4 maps to physical IEC connector panel 0-4

/** Relay channel index for ventilation fan: low rpm */
constexpr int relay_channel_idx_ventilation_fan_channel_low_rpm = 0;

/** Relay channel index for ventilation fan: high rpm */
constexpr int relay_channel_idx_ventilation_fan_channel_high_rpm = 1;

/** Relay channel index for upper full spectrum light */
constexpr int relay_channel_idx_upper_full_spectrum_light_channel = 2;

/** Relay channel index for lower full spectrum light */
constexpr int relay_channel_idx_lower_full_spectrum_light_channel = 3;

/** Relay channel index for wind simulation fan */
constexpr int relay_channel_idx_wind_simulation_fan_channel = 4;

// Physical IEC connector panel 5 is used for daisy chaining mains power

// Any other relay functions is out of scope for the chassi controller

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
