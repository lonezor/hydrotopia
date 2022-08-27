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

/** @file
 * request_handling/include/public/udp_server/request_handling/error_code.hpp
 *  @brief Categories for exception handling
 */

#include <chrono>
#include <memory>
#include <string>
#include <system_error>

//-------------------------------------------------------------------------------------------------------------------

namespace hydroctrl {
namespace common {

//-------------------------------------------------------------------------------------------------------------------
/** Request handling error codes */
enum class hydroctrl_error
{
    /** Target already registered */
    target_already_registered,
};

//-------------------------------------------------------------------------------------------------------------------

/** @brief Create error_code from hydroctrl_error
 */
std::error_code make_error_code(hydroctrl_error e);

//-------------------------------------------------------------------------------------------------------------------

/** Exception code as string */
std::string ex_code_str(int code);

} // namespace common
} // namespace hydroctrl

namespace std {
template <>
struct is_error_code_enum<hydroctrl::common::hydroctrl_error> : public true_type
{
};
} // namespace std

//-------------------------------------------------------------------------------------------------------------------