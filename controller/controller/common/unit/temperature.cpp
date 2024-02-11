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

#include <common/unit/temperature.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

temperature::temperature() {}

//---------------------------------------------------------------------------------------------------------------------

temperature::temperature(temp_unit unit, double value)
    : unit_(unit), value_(value)
{
}

//---------------------------------------------------------------------------------------------------------------------

void temperature::set(temp_unit unit, double value)
{
    unit_ = unit;
    value_ = value;
}

//---------------------------------------------------------------------------------------------------------------------

temperature::temp_unit temperature::unit() { return unit_; }

//---------------------------------------------------------------------------------------------------------------------

double temperature::value() { return value_; }

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl