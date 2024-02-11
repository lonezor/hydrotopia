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

#include <common/power_consumption.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

std::string power_consumption_profile_str(power_consumption_profile profile)
{
    switch (profile) {
    case power_consumption_profile::continuous:
        return "Continuous";
    case power_consumption_profile::high:
        return "High";
    case power_consumption_profile::medium:
        return "Medium";
    case power_consumption_profile::low:
        return "Low";
    case power_consumption_profile::emergency:
        return "Emergency";
    case power_consumption_profile::off:
        return "Off";
    case power_consumption_profile::unknown:
    default:
        return "";
    }
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
