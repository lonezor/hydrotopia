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

#include <common/channel/subsystem/main/water_pump_channel.hpp>
#include <common/log.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

water_pump_channel::water_pump_channel(
    common::electrical_system electrical_system,
    common::channel_type channel_type,
    std::shared_ptr<common::controller_ctx> ctx)
    : channel::channel(common::subsystem::main, electrical_system, channel_type,
                       ctx)
{}

//---------------------------------------------------------------------------------------------------------------------

void water_pump_channel::channel_activation_cb(common::task_context task_ctx,
                                               water_pump_channel *_this)
{
    common::log(common::log_level::log_level_debug,
                "[water_pump_channel::channel_activation_cb]");
    _this->activate();
}

//---------------------------------------------------------------------------------------------------------------------

void water_pump_channel::channel_deactivation_cb(common::task_context task_ctx,
                                                 water_pump_channel *_this)
{
    if (task_ctx.id != _this->latest_deactivation_task_id_) {
        common::log(
            common::log_level::log_level_debug,
            "[water_pump_channel::channel_deactivation_cb] ignore outdated "
            "timer callback");
        return;
    }

    common::log(common::log_level::log_level_debug,
                "[water_pump_channel::channel_deactivation_cb]");
    _this->deactivate();
}

//---------------------------------------------------------------------------------------------------------------------

void water_pump_channel::hourly_tick()
{
    common::log(common::log_level::log_level_debug,
                "[water_pump_channel::hourly_tick]");

    auto hour = ctx()->clock->hour();

    // No action during night time: 00:00 - 05:59
    if (hour >= midnight && hour < six_in_the_morning) {
        return;
    }

    // No action when power profile is 'off'
    if (power_profile() == common::power_consumption_profile::off) {
        return;
    }

    // For continuous hourly operation, no reset is needed. The activation of
    // this power profile is always initiated by the user. Thus no need for
    // randomized delay. It is not timer driven.
    if (power_profile() == common::power_consumption_profile::high) {
        return;
    }

    auto start_delay = random_start_delay();

    auto bf = std::bind(&channel_activation_cb, std::placeholders::_1, this);
    ctx()->task_scheduler->register_single_task(start_delay, bf);
}

//---------------------------------------------------------------------------------------------------------------------

void water_pump_channel::activate()
{
    /* Invocation context:
     * - User triggered activation via user interface (immediate activation with
     * no channel concurrency, also the initial activation)
     * - Hourly trigger via system clock timer (evently spread delayed
     * activation due to channel concurrency)
     */

    common::log(common::log_level::log_level_debug,
                "[water_pump_channel::activate]");

    // activate relay
    auto indexes = relay_indexes();
    if (indexes.size() == 1) {
        ctx()->relay_module->activate(indexes.at(0));
    } else {
        throw std::runtime_error(
            "[water_pump_channel::activate] critical error");
    }

    // Set deactivation timer, but not for durations filling up the entire time
    // window
    if (power_profile() != common::power_consumption_profile::high) {
        auto duration = determine_activation_duration(power_profile());
        auto bf =
            std::bind(&channel_deactivation_cb, std::placeholders::_1, this);
        latest_deactivation_task_id_ =
            ctx()->task_scheduler->register_single_task(duration, bf);
    }
}

//---------------------------------------------------------------------------------------------------------------------

void water_pump_channel::deactivate()
{
    /* Invocation context:
     * - User triggered deactivation via user interface (immediate deactivation
     * with no channel concurrency, also the initial activation)
     * - Hourly trigger via system clock timer (evently spread delayed
     * deactivation due to channel concurrency)
     */

    common::log(common::log_level::log_level_debug,
                "[water_pump_channel::deactivate]");

    // deactivate relay
    auto indexes = relay_indexes();
    if (indexes.size() == 1) {
        ctx()->relay_module->deactivate(indexes.at(0));
    } else {
        throw std::runtime_error(
            "[water_pump_channel::deactivate] critical error");
    }
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
