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

#include <string>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

int extract_integer(const std::string &str, const std::string &expression);

//-------------------------------------------------------------------------------------------------------------------

double extract_double(const std::string &str, const std::string &expression);

//-------------------------------------------------------------------------------------------------------------------

std::string extract_string(const std::string &str,
                           const std::string &expression);

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl