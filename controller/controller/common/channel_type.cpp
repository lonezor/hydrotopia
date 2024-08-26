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
    case channel_type::upper_full_spectrum_light:
        return "Upper Full Spectrum Light";
    case channel_type::lower_full_spectrum_light:
        return "Lower Full Spectrum Light";
    case channel_type::drip_irrigation:
        return "Drip Irrigation";
    case channel_type::ventilation_fan:
        return "Ventilation Fan";
    case channel_type::wind_simulation_fan:
        return "Wind simulator fan";
    case channel_type::unknown:
    default:
        return "";
    }
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
