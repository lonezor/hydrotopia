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

#include <cstddef>

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

  private:
    rc_relay_channel_t index_to_channel_type(int index);

    size_t size_{relay_module_size};
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
