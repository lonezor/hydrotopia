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

#include <common/ventilation_fan.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

std::string ventilation_fan_mode_str(ventilation_fan_mode mode)
{
    switch (mode) {
    case ventilation_fan_mode::low:
        return "Low";
    case ventilation_fan_mode::high:
        return "High";
    case ventilation_fan_mode::automatic:
        return "Auto";
    case ventilation_fan_mode::none:
    default:
        return "";
    }
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
