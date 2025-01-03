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

/** Channel type */
enum class channel_type
{
    unknown,

    /** Upper full spectrum light */
    upper_full_spectrum_light,

    /** Lower full spectrum light */
    lower_full_spectrum_light,

    /** Drip irrigation */
    drip_irrigation,

    /** Ventilation fan */
    ventilation_fan,

    /** Wind simulator fan */
    wind_simulation_fan,
};

//---------------------------------------------------------------------------------------------------------------------

/** Channel type as string */
std::string channel_type_str(channel_type type);

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
