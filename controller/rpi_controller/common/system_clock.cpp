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

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <common/system_clock.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

system_clock::system_clock()
{
    // Ensure there is no initial transition of hour, minute and seconds
    update();
    update(); // prev variables also updated
}

//---------------------------------------------------------------------------------------------------------------------

void system_clock::update()
{
    prev_tick_hour_ = hour_;
    prev_tick_minute_ = minute_;
    prev_tick_second_ = second_;

    time_t now = ::time(0);
    struct tm tstruct;
    char buffer[80];
    tstruct = *localtime(&now);

    // Year
    strftime(buffer, sizeof(buffer), "%Y", &tstruct);
    year_ = std::atoi(buffer);

    // Month
    strftime(buffer, sizeof(buffer), "%m", &tstruct);
    month_ = std::atoi(buffer);

    // Day
    strftime(buffer, sizeof(buffer), "%d", &tstruct);
    day_ = std::atoi(buffer);

    // Hour
    strftime(buffer, sizeof(buffer), "%H", &tstruct);
    hour_ = std::atoi(buffer);

    // Minute
    strftime(buffer, sizeof(buffer), "%M", &tstruct);
    minute_ = std::atoi(buffer);

    // Second
    strftime(buffer, sizeof(buffer), "%S", &tstruct);
    second_ = std::atoi(buffer);

    printf("update now %u %s %s\n", now, date().c_str(), time_full().c_str());
}

//---------------------------------------------------------------------------------------------------------------------

void system_clock::tick() { update(); }

//---------------------------------------------------------------------------------------------------------------------

int system_clock::year() { return year_; }

//---------------------------------------------------------------------------------------------------------------------

int system_clock::month() { return month_; }

//---------------------------------------------------------------------------------------------------------------------

int system_clock::day() { return day_; }

//---------------------------------------------------------------------------------------------------------------------

int system_clock::hour() { return hour_; }

//---------------------------------------------------------------------------------------------------------------------

int system_clock::minute() { return minute_; }

//---------------------------------------------------------------------------------------------------------------------

int system_clock::second() { return second_; }

//---------------------------------------------------------------------------------------------------------------------

std::string system_clock::date()
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%d-%02d-%02d", year_, month_, day_);
    return std::string(buffer);
}

//---------------------------------------------------------------------------------------------------------------------

std::string system_clock::time()
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hour_, minute_);
    return std::string(buffer);
}

//---------------------------------------------------------------------------------------------------------------------

std::string system_clock::time_full()
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hour_, minute_, second_);
    return std::string(buffer);
}

//---------------------------------------------------------------------------------------------------------------------

bool system_clock::hour_transition() { return hour_ != prev_tick_hour_; }

//---------------------------------------------------------------------------------------------------------------------

bool system_clock::minute_transition() { return minute_ != prev_tick_minute_; }

//---------------------------------------------------------------------------------------------------------------------

bool system_clock::second_transition() { return second_ != prev_tick_second_; }

//---------------------------------------------------------------------------------------------------------------------

int system_clock::hourly_seconds_remaining()
{
    int remaining = 3600;

    remaining -= minute_ * 60;
    remaining -= second_;

    if (remaining < 0) {
        remaining = 0;
    }

    return remaining;
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl