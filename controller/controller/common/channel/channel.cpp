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

#include <random>
#include <sstream>

#include <common/channel/channel.hpp>
#include <common/log.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

channel::channel(common::subsystem subsystem,
                 common::channel_type channel_type,
                 std::shared_ptr<common::controller_ctx> ctx)
    : subsystem_(subsystem), channel_type_(channel_type), ctx_(ctx)
{
}

//---------------------------------------------------------------------------------------------------------------------

common::subsystem channel::subsystem() { return subsystem_; }

//---------------------------------------------------------------------------------------------------------------------

common::channel_type channel::channel_type() { return channel_type_; }

//---------------------------------------------------------------------------------------------------------------------

std::shared_ptr<common::controller_ctx> channel::ctx() { return ctx_; }

//---------------------------------------------------------------------------------------------------------------------

void channel::set_relay_module_idx(int idx)
{
    relay_indexes_.clear();
    relay_indexes_.emplace_back(idx);
}

//---------------------------------------------------------------------------------------------------------------------

void channel::set_relay_module_idx(std::vector<int> indexes)
{
    relay_indexes_ = indexes;
}

//---------------------------------------------------------------------------------------------------------------------

std::vector<int> channel::relay_indexes() { return relay_indexes_; }

//---------------------------------------------------------------------------------------------------------------------

void channel::set_power_consumption_profile(
    common::power_consumption_profile profile)
{
    std::stringstream ss_msg;
    ss_msg << "[" << common::channel_type_str(channel::channel::channel_type())
           << "] power profile '"
           << common::power_consumption_profile_str(profile) << "'";
    common::log(common::log_level::log_level_info, ss_msg.str());

    power_profile_ = profile;
}

//---------------------------------------------------------------------------------------------------------------------

common::power_consumption_profile channel::power_profile()
{
    return power_profile_;
}

//---------------------------------------------------------------------------------------------------------------------

std::chrono::milliseconds channel::random_start_delay()
{
    std::random_device rd;
    std::uniform_int_distribution<int> dist(
        0, static_cast<int>(hourly_reset_randomization_period.count() - 1));
    auto value = dist(rd);

    return std::chrono::milliseconds(value);
}

//---------------------------------------------------------------------------------------------------------------------

std::chrono::milliseconds channel::hourly_remaining()
{
    auto seconds = ctx_->clock->hourly_seconds_remaining();
    auto milliseconds = std::chrono::milliseconds(seconds * 1000);
    return milliseconds;
}

//---------------------------------------------------------------------------------------------------------------------

std::chrono::milliseconds channel::duration_of_hourly_power_profile(
    common::power_consumption_profile profile)
{
    switch (profile) {
    case common::power_consumption_profile::continuous:
        return std::chrono::milliseconds(
            common::hourly_power_consumption_profile_continuous.count() * 1000);
    case common::power_consumption_profile::high:
        return std::chrono::milliseconds(
            common::hourly_power_consumption_profile_high.count() * 1000);
    case common::power_consumption_profile::medium:
        return std::chrono::milliseconds(
            common::hourly_power_consumption_profile_medium.count() * 1000);
    case common::power_consumption_profile::low:
        return std::chrono::milliseconds(
            common::hourly_power_consumption_profile_low.count() * 1000);
    case common::power_consumption_profile::emergency:
        return std::chrono::milliseconds(
            common::hourly_power_consumption_profile_emergency.count() * 1000);
    case common::power_consumption_profile::off:
    default:
        return std::chrono::milliseconds(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------

std::chrono::milliseconds channel::determine_activation_duration(
    common::power_consumption_profile profile)
{
    if (profile == common::power_consumption_profile::continuous) {
        return std::chrono::milliseconds(0);
    }

    auto duration = duration_of_hourly_power_profile(profile);
    auto hourly_budget = hourly_remaining();

    // Define shorter time window to ensure channel will be turned off before
    // new window starts
    constexpr std::chrono::milliseconds safety_duration =
        std::chrono::milliseconds(60000);
    auto time_window = hourly_budget - safety_duration;

    if (duration >= time_window) {
        duration = time_window;
    }

    std::stringstream ss_msg;
    ss_msg << "[channel::determine_activation_duration] duration "
           << duration.count();
    common::log(common::log_level::log_level_debug, ss_msg.str());

    return duration;
}

//---------------------------------------------------------------------------------------------------------------------

void channel::hourly_tick()
{
    // No action for this channel
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
