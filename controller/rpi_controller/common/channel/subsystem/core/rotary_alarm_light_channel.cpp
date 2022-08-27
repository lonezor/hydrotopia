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

#include <sstream>

#include <common/channel/subsystem/core/rotary_alarm_light_channel.hpp>
#include <common/log.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

rotary_alarm_light_channel::rotary_alarm_light_channel(
    std::shared_ptr<common::controller_ctx> ctx)
    : channel::channel::channel(common::subsystem::core,
                                common::channel_type::rotary_alarm_light, ctx)
{
}

//---------------------------------------------------------------------------------------------------------------------

void rotary_alarm_light_channel::activate()
{
    // activate relay
    auto indexes = relay_indexes();
    if (indexes.size() == 1) {
        ctx()->relay_module->activate(indexes.at(0));
    } else {
        throw std::runtime_error(
            "[rotary_alarm_light_channel::activate] critical error");
    }
}

//---------------------------------------------------------------------------------------------------------------------

void rotary_alarm_light_channel::deactivate()
{
    // deactivate relay
    auto indexes = relay_indexes();
    if (indexes.size() == 1) {
        ctx()->relay_module->deactivate(indexes.at(0));
    } else {
        throw std::runtime_error(
            "[rotary_alarm_light_channel::deactivate] critical error");
    }
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
