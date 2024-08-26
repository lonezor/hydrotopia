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

#include <common/channel/subsystem/main/drip_irrigation_channel.hpp>
#include <common/log.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

drip_irrigation_channel::drip_irrigation_channel(
    common::channel_type channel_type,
    std::shared_ptr<common::controller_ctx> ctx)
    : channel::channel::channel(common::subsystem::main, 
                                channel_type, ctx)
{}

//---------------------------------------------------------------------------------------------------------------------

std::chrono::hours drip_irrigation_channel::duration_of_daily_power_profile(
    common::power_consumption_profile profile)
{
    switch (profile) {
    case common::power_consumption_profile::high:
        return common::light_power_consumption_profile_high;
    case common::power_consumption_profile::medium:
        return common::light_power_consumption_profile_medium;
    case common::power_consumption_profile::low:
        return common::light_power_consumption_profile_low;
    case common::power_consumption_profile::emergency:
        return common::light_power_consumption_profile_emergency;
    case common::power_consumption_profile::off:
    default:
        return std::chrono::hours(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------

void drip_irrigation_channel::hourly_tick()
{
    auto hour = ctx()->clock->hour();

    // If not active, follow schedule
    if (!activated_) {
        // Daily activation
        if (hour == six_in_the_morning) {
            if (power_profile() != common::power_consumption_profile::off) {
                activate();
            }
        }

        return;
    }

    hours_activated_++;

    // Deactivate when reaching power profile duration
    auto threshold = duration_of_daily_power_profile(power_profile());
    if (hours_activated_ >= threshold) {
        deactivate();
    }

    // The user may have toggled the light to get another cycle
    // However, force light off during the night to avoid too
    // long light exposure.
    if (hour == midnight) {
        deactivate();
    }
}

//---------------------------------------------------------------------------------------------------------------------

void drip_irrigation_channel::activate()
{
    // activate relay
    auto indexes = relay_indexes();
    if (indexes.size() == 1) {
        ctx()->relay_module->activate(indexes.at(0));
    } else {
        throw std::runtime_error(
            "[drip_irrigation_channel::activate] critical error");
    }

    activated_ = true;
    hours_activated_ = std::chrono::hours(0);
}

//---------------------------------------------------------------------------------------------------------------------

void drip_irrigation_channel::deactivate()
{
    // deactivate relay
    auto indexes = relay_indexes();
    if (indexes.size() == 1) {
        ctx()->relay_module->deactivate(indexes.at(0));
    } else {
        throw std::runtime_error(
            "[drip_irrigation_channel::deactivate] critical error");
    }

    activated_ = false;
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
