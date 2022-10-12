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
    common::electrical_system electrical_system,
    std::shared_ptr<common::controller_ctx> ctx)
    : channel::channel(common::subsystem::main, electrical_system,
                       common::channel_type::ventilation_fan, ctx)
{}

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

    bool update_needed = channel_update_needed();

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
    if (power_profile() == common::power_consumption_profile::high &&
        !update_needed) {
        return;
    }

    auto start_delay = random_start_delay();

    auto bf = std::bind(&channel_activation_cb, std::placeholders::_1, this);
    ctx()->task_scheduler->register_single_task(start_delay, bf);
}

//---------------------------------------------------------------------------------------------------------------------

bool ventilation_fan_channel::channel_update_needed()
{
    auto cabinet_status = ctx()->cabinet_status;

    // Manual mode
    if (fan_mode_ == common::ventilation_fan_mode::low ||
        fan_mode_ == common::ventilation_fan_mode::high) {
        return true;
    }

    // When not requesting 'off', fan RPM setting must be set
    if (fan_rpm_setting_ == common::ventilation_fan_mode::none) {
        return true;
    }

    // Upper limit reached but still using low RPM
    if (fan_rpm_setting_ == common::ventilation_fan_mode::low &&
        cabinet_status.cabinet_temperature.value() >
            ventilation_fan_high_rpm_upper_threshold) {
        return true;
    }

    // Lower limit reached but still using high RPM
    if (fan_rpm_setting_ == common::ventilation_fan_mode::high &&
        cabinet_status.cabinet_temperature.value() <
            ventilation_fan_high_rpm_lower_threshold) {
        return true;
    }

    return false;
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

    // Do not go through with deactivation + activation routine unless necessary
    bool update_needed = channel_update_needed();
    if (!update_needed) {
        return;
    }

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
        fan_rpm_setting_ = common::ventilation_fan_mode::low;
        std::stringstream ss_msg;
        ss_msg << "[ventilation_fan_channel::activate] [manual] setting RPM to "
                  "low";
        common::log(common::log_level::log_level_debug, ss_msg.str());
        break;
    }
    case common::ventilation_fan_mode::high: {
        ctx()->relay_module->activate(indexes.at(1));
        fan_rpm_setting_ = common::ventilation_fan_mode::high;
        std::stringstream ss_msg;
        ss_msg << "[ventilation_fan_channel::activate] [manual] setting RPM to "
                  "high";
        common::log(common::log_level::log_level_debug, ss_msg.str());
        break;
    }
    case common::ventilation_fan_mode::automatic: {
        auto cabinet_status = ctx()->cabinet_status;

        // Start on lowest setting
        if (fan_rpm_setting_ == common::ventilation_fan_mode::none) {
            ctx()->relay_module->activate(indexes.at(0));
            fan_rpm_setting_ = common::ventilation_fan_mode::low;
            std::stringstream ss_msg;
            ss_msg << "[ventilation_fan_channel::activate] [automatic] setting "
                      "initial RPM to low";
            common::log(common::log_level::log_level_debug, ss_msg.str());
            break;
        }

        // Exceeding upper threshold
        if (fan_rpm_setting_ == common::ventilation_fan_mode::low &&
            cabinet_status.cabinet_temperature.value() >
                ventilation_fan_high_rpm_upper_threshold) {
            ctx()->relay_module->activate(indexes.at(1));
            fan_rpm_setting_ = common::ventilation_fan_mode::high;
            std::stringstream ss_msg;
            ss_msg << "[ventilation_fan_channel::activate] [automatic] setting "
                      "RPM to high";
            common::log(common::log_level::log_level_debug, ss_msg.str());
            break;
        }

        // Lower limit reached but still using high RPM
        if (fan_rpm_setting_ == common::ventilation_fan_mode::high &&
            cabinet_status.cabinet_temperature.value() <
                ventilation_fan_high_rpm_lower_threshold) {
            ctx()->relay_module->activate(indexes.at(0));
            fan_rpm_setting_ = common::ventilation_fan_mode::low;
            std::stringstream ss_msg;
            ss_msg << "[ventilation_fan_channel::activate] [automatic] setting "
                      "RPM to low";
            common::log(common::log_level::log_level_debug, ss_msg.str());
            break;
        }
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
