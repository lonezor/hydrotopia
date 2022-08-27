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

#include <exception>
#include <sstream>
#include <thread>

#include <common/channel/subsystem/main/ventilation_fan_channel.hpp>
#include <common/log.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

ventilation_fan_channel::ventilation_fan_channel(
    std::shared_ptr<common::controller_ctx> ctx)
    : channel::channel::channel(common::subsystem::main,
                                common::channel_type::ventilation_fan, ctx)
{
}

//---------------------------------------------------------------------------------------------------------------------

void ventilation_fan_channel::set_fan_mode(common::ventilation_fan_mode mode)
{
    fan_mode_ = mode;
}

//---------------------------------------------------------------------------------------------------------------------

common::ventilation_fan_mode ventilation_fan_channel::fan_mode()
{
    return fan_mode_;
}

//---------------------------------------------------------------------------------------------------------------------

void ventilation_fan_channel::channel_activation_cb(
    common::task_context task_ctx, ventilation_fan_channel *_this)
{
    common::log(common::log_level::log_level_debug,
                "[ventilation_fan_channel::channel_activation_cb]");
    _this->activate();
}

//---------------------------------------------------------------------------------------------------------------------

void ventilation_fan_channel::channel_deactivation_cb(
    common::task_context task_ctx, ventilation_fan_channel *_this)
{
    if (task_ctx.id != _this->latest_deactivation_task_id_) {
        common::log(common::log_level::log_level_debug,
                    "[ventilation_fan_channel::channel_deactivation_cb] ignore "
                    "outdated timer callback");
        return;
    }

    common::log(common::log_level::log_level_debug,
                "[ventilation_fan_channel::channel_deactivation_cb]");
    _this->deactivate();
}

//---------------------------------------------------------------------------------------------------------------------

void ventilation_fan_channel::hourly_tick()
{
    common::log(common::log_level::log_level_debug,
                "[ventilation_fan_channel::hourly_tick]");

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

void ventilation_fan_channel::activate()
{
    /* Invocation context:
     * - User triggered activation via user interface (immediate activation with
     * no channel concurrency, also the initial activation)
     * - Hourly trigger via system clock timer (evently spread delayed
     * activation due to channel concurrency)
     */

    common::log(common::log_level::log_level_debug,
                "[ventilation_fan_channel::activate]");

    //*** activate relay ***
    auto indexes = relay_indexes();

    // The two relay channels are strictly mutually exclusive
    // First turn off relay
    deactivate();

    switch (fan_mode_) {
    case common::ventilation_fan_mode::low: {
        ctx()->relay_module->activate(indexes.at(0));
        break;
    }
    case common::ventilation_fan_mode::high: {
        ctx()->relay_module->activate(indexes.at(1));
        break;
    }
    case common::ventilation_fan_mode::automatic: {
        // Todo: temperature input
        ctx()->relay_module->activate(indexes.at(0));
        break;
    }
    default:
        throw std::runtime_error("[ventilation_fan_channel::activate] "
                                 "unexpected ventilation fan mode");
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

void ventilation_fan_channel::deactivate()
{
    /* Invocation context:
     * - User triggered deactivation via user interface (immediate deactivation
     * with no channel concurrency, also the initial activation)
     * - Hourly trigger via system clock timer (evently spread delayed
     * deactivation due to channel concurrency)
     */

    common::log(common::log_level::log_level_debug,
                "[ventilation_fan_channel::deactivate]");

    // deactivate relay channels: it is always safe to cut power
    auto indexes = relay_indexes();
    if (indexes.size() == 2) {
        ctx()->relay_module->deactivate(indexes.at(0));
        ctx()->relay_module->deactivate(indexes.at(1));
    } else {
        throw std::runtime_error(
            "[ventilation_fan_channel::deactivate] critical error");
    }

    // Sleep 2 seconds to ensure power is off
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
