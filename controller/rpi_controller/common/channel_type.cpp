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

#include <common/channel_type.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

/** Channel type as string */
std::string channel_type_str(channel_type type)
{
    switch (type) {
    case channel_type::rotary_alarm_light:
        return "Rotary Alarm Light";
    case channel_type::ventilation_fan:
        return "Ventilation Fan";
    case channel_type::upper_full_spectrum_light:
        return "Upper Full Spectrum Light";
    case channel_type::lower_full_spectrum_light:
        return "Lower Full Spectrum Light";
    case channel_type::upper_water_pump_1:
        return "Upper Water Pump #1";
    case channel_type::upper_water_pump_2:
        return "Upper Water Pump #2";
    case channel_type::lower_water_pump_1:
        return "Lower Water Pump #1";
    case channel_type::lower_water_pump_2:
        return "Lower Water Pump #2";
    case channel_type::lower_water_pump_3:
        return "Lower Water Pump #3";
    case channel_type::unknown:
    default:
        return "";
    }
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
