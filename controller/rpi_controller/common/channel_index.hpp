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

/******************************************************************************************/
/*********************************** AC 220-240 VOLT **************************************/
/******************************************************************************************/

/** Relay channel index for rotary alarm light */
constexpr int relay_channel_idx_rotary_alarm_light_channel = 0;

/** Relay channel index for ventilation fan: low rpm */
constexpr int relay_channel_idx_ventilation_fan_channel_low_rpm = 1;

/** Relay channel index for ventilation fan: high rpm */
constexpr int relay_channel_idx_ventilation_fan_channel_high_rpm = 2;

/** Relay channel index for upper full spectrum light */
constexpr int relay_channel_idx_upper_full_spectrum_light_channel = 3;

/** Relay channel index for lower full spectrum light */
constexpr int relay_channel_idx_lower_full_spectrum_light_channel = 4;

/** Relay channel index for lower water pump #1 */
constexpr int relay_channel_idx_lower_water_pump_1 = 5;

/** Relay channel index for lower water pump #2 */
constexpr int relay_channel_idx_lower_water_pump_2 = 6;

/** Relay channel index for lower water pump #3 */
constexpr int relay_channel_idx_lower_water_pump_3 = 7;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_1 = 8;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_2 = 9;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_3 = 10;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_4 = 11;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_5 = 12;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_6 = 13;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_7 = 14;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_8 = 15;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_9 = 16;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_10 = 17;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_11 = 18;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_12 = 19;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_13 = 20;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_14 = 21;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_15 = 22;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_16 = 23;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_17 = 24;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_18 = 25;

/** Relay channel index for step-down voltage transformer */
constexpr int relay_channel_idx_step_down_voltage_transformer = 26;

/******************************************************************************************/
/*********************************** AC 110-120 VOLT **************************************/
/******************************************************************************************/

/** Relay channel index for upper water pump #1 */
constexpr int relay_channel_idx_upper_water_pump_1 = 27;

/** Relay channel index for upper water pump #2 */
constexpr int relay_channel_idx_upper_water_pump_2 = 28;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_19 = 29;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_20 = 30;

/** Relay channel index: reserved slot */
constexpr int relay_channel_idx_reserved_21 = 31;

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
