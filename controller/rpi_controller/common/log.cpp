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

#include <array>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/time.h>

#include <common/log.hpp>

namespace hydroctrl {
namespace common {

//-------------------------------------------------------------------------------------------------------------------

static log_level g_log_level = log_level_default;

//-------------------------------------------------------------------------------------------------------------------

constexpr int log_level_str_array_size = 8;
static constexpr const std::array<const char *, log_level_str_array_size>
    g_log_level_str{
        "LOG_EMERG",   "LOG_ALERT",  "LOG_CRIT", "LOG_ERR",
        "LOG_WARNING", "LOG_NOTICE", "LOG_INFO", "LOG_DEBUG",
    };

//-------------------------------------------------------------------------------------------------------------------

void set_log_level(log_level level) { g_log_level = level; }

//-------------------------------------------------------------------------------------------------------------------

log_level get_log_level() { return g_log_level; }

//-------------------------------------------------------------------------------------------------------------------

void log(log_level level, const std::string &msg)
{
    if (level > g_log_level) {
        return;
    }

    // Use gettimeofday() and strftime() to get custom timestamp string
    struct timeval ts
    {};
    constexpr int str_size = 128;
    std::array<char, str_size> timestamp_str{};

    gettimeofday(&ts, nullptr);

    std::stringstream ss_format;

    ss_format << "[%Y-%m-%d %H:%M:%S." << std::setfill('0') << std::setw(3)
              << static_cast<int>(static_cast<double>(ts.tv_usec) / 1000)
              << "]";

// The whole idea with ss_format is that it is not a string literal and defined
// in runtime
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
    strftime(timestamp_str.data(), timestamp_str.size(),
             ss_format.str().c_str(), localtime(&ts.tv_sec));
#pragma GCC diagnostic pop

    std::cerr << std::string(timestamp_str.data()) << " ["
              << g_log_level_str.at(static_cast<int>(level)) << "] " << msg
              << std::endl
              << std::flush;
}

//-------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl