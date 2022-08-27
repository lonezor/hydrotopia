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

#include <regex>

#include <common/string_processing/regex.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

int extract_integer(const std::string &str, const std::string &expression)
{
    int value = -1;
    auto re = std::regex(expression);
    std::smatch match;

    if (std::regex_search(str, match, re) && match.size() > 1) {
        value = std::stoi(match.str(1));
    }

    return value;
}

//-------------------------------------------------------------------------------------------------------------------

double extract_double(const std::string &str, const std::string &expression)
{
    double value = -1;
    auto re = std::regex(expression);
    std::smatch match;

    if (std::regex_search(str, match, re) && match.size() > 1) {
        value = std::stod(match.str(1));
    }

    return value;
}

//-------------------------------------------------------------------------------------------------------------------

std::string extract_string(const std::string &str,
                           const std::string &expression)
{
    auto re = std::regex(expression);
    std::smatch match;

    if (std::regex_search(str, match, re) && match.size() > 1) {
        return match.str(1);
    }

    return "";
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl