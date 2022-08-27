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

class system_clock
{
  public:
    /** Constructor */
    system_clock();

    /** 500 ms timer tick */
    void tick();

    /** Current year (YYYY) */
    int year();

    /** Current month (1-12) */
    int month();

    /** Current day of month (1-31) */
    int day();

    /** Current hour (24h format: 0-23) */
    int hour();

    /** Current minute (0-59) */
    int minute();

    /** Current second (0-60 including occational leap second) */
    int second();

    /** Current date YYYY-MM-DD */
    std::string date();

    /** Current time HH:MM */
    std::string time();

    /** Current time HH:MM:SS */
    std::string time_full();

    /** Last tick transitioned to new hour (trigger point) */
    bool hour_transition();

    /** Last tick transitioned to new minute (trigger point) */
    bool minute_transition();

    /** Last tick transitioned to new second (trigger point) */
    bool second_transition();

    /** Time in seconds left in the current hour */
    int hourly_seconds_remaining();

  private:
    /** Update */
    void update();

    int year_{0};

    int month_{0};

    int day_{0};

    int hour_{0};

    int minute_{0};

    int second_{0};

    int prev_tick_hour_{0};

    int prev_tick_minute_{0};

    int prev_tick_second_{0};
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl