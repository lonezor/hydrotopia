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

#include <cstddef>
#include <stdexcept>

#include <rc/relay.h>

#include <common/log.hpp>
#include <common/relay_module/relay_module.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

relay_module::relay_module()
{
    rc_relay_channel_init();
    clear();
}

//---------------------------------------------------------------------------------------------------------------------

int relay_module::size() { return size_; }

//---------------------------------------------------------------------------------------------------------------------

rc_relay_channel_t relay_module::index_to_channel_type(int index)
{
    rc_relay_channel_t ch;

    switch (index) {
    case 0:
        ch = rc_relay_channel_01;
        break;
    case 1:
        ch = rc_relay_channel_02;
        break;
    case 2:
        ch = rc_relay_channel_03;
        break;
    case 3:
        ch = rc_relay_channel_04;
        break;
    case 4:
        ch = rc_relay_channel_05;
        break;
    case 5:
        ch = rc_relay_channel_06;
        break;
    case 6:
        ch = rc_relay_channel_07;
        break;
    case 7:
        ch = rc_relay_channel_08;
        break;
    case 8:
        ch = rc_relay_channel_09;
        break;
    case 9:
        ch = rc_relay_channel_10;
        break;
    case 10:
        ch = rc_relay_channel_11;
        break;
    case 11:
        ch = rc_relay_channel_12;
        break;
    case 12:
        ch = rc_relay_channel_13;
        break;
    case 13:
        ch = rc_relay_channel_14;
        break;
    case 14:
        ch = rc_relay_channel_15;
        break;
    case 15:
        ch = rc_relay_channel_16;
        break;
    default:
        throw std::runtime_error(
            "[relay_module::activate] failed to map index to channel type");
    }

    return ch;
}

//---------------------------------------------------------------------------------------------------------------------

void relay_module::activate(int index)
{
#ifdef HC_RELAY_MODULE_ENABLED
    common::log(common::log_level::log_level_debug, "[relay_module::activate]");
    rc_relay_channel_t ch = relay_module::index_to_channel_type(index);
    rc_relay_channel_set(rc_relay_port_a, ch, true);
#else  // HC_RELAY_MODULE_ENABLED
    common::log(common::log_level::log_level_notice,
                "[relay_module::activate] stubbed");
#endif // HC_RELAY_MODULE_ENABLED
}

//---------------------------------------------------------------------------------------------------------------------

void relay_module::deactivate(int index)
{
#ifdef HC_RELAY_MODULE_ENABLED
    common::log(common::log_level::log_level_debug,
                "[relay_module::deactivate]");
    rc_relay_channel_t ch = relay_module::index_to_channel_type(index);
    rc_relay_channel_set(rc_relay_port_a, ch, false);
#else  // HC_RELAY_MODULE_ENABLED
    common::log(common::log_level::log_level_notice,
                "[relay_module::deactivate] stubbed");
#endif // HC_RELAY_MODULE_ENABLED
}

//---------------------------------------------------------------------------------------------------------------------

void relay_module::clear()
{
#ifdef HC_RELAY_MODULE_ENABLED
    common::log(common::log_level::log_level_debug, "[relay_module::clear]");

    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_01, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_02, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_03, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_04, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_05, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_06, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_07, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_08, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_09, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_10, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_11, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_12, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_13, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_14, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_15, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_16, false);

    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_01, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_02, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_03, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_04, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_05, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_06, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_07, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_08, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_09, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_10, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_11, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_12, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_13, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_14, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_15, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_16, false);
#else  // HC_RELAY_MODULE_ENABLED
    common::log(common::log_level::log_level_notice,
                "[relay_module::clear] stubbed");
#endif // HC_RELAY_MODULE_ENABLED
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
