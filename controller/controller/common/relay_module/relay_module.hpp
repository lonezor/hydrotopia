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

#include <chrono>
#include <cstddef>
#include <vector>

#include <rc/relay.h>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

constexpr int relay_module_size = 32;

//---------------------------------------------------------------------------------------------------------------------

/** Relay module */
class relay_module
{
  public:
    /** Constructor */
    relay_module();

    /** Number of relays */
    int size();

    /** Activate relay */
    void activate(int index);

    /** Deactivate relay */
    void deactivate(int index);

    /** Clear all channels */
    void clear();

    /** Get stats: string representation */
    std::string stats();

  private:
    rc_relay_port_t index_to_relay_port(int index);

    rc_relay_channel_t index_to_channel_type(int index);

    /**
     *  @brief Relay activation state
     *
     * This guarantees that the relay module is only accessed
     * in logically correct order. This avoids communicating
     * over the serial bus.
     */
    std::vector<bool> activation_state_;

    /** Relay module size */
    size_t size_{relay_module_size};

    /** Activation histogram */
    std::vector<uint64_t> activation_histogram_;

    /** Activation timepoint references */
    std::vector<std::chrono::steady_clock::time_point>
        activation_timepoint_refs_;

    /** Duration histogram */
    std::vector<std::chrono::milliseconds> duration_histogram_;
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
