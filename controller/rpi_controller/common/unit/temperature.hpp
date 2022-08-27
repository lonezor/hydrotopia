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

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

/** Temperature */
class temperature
{
  public:
    /** Temperature unit */
    enum class temp_unit
    {
        unknown,

        /** Unit of temperature on the Celsius scale */
        celcius,
    };

    temperature(temp_unit unit, double value);

    void set(temp_unit unit, double value);

    temp_unit unit();
    double value();

    /** Temperature unit */
    temp_unit unit_;

    /** Value, expressed in current temperature unit */
    double value_;
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl