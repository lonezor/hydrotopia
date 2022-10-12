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

#include <memory>
#include <vector>

#include <common/channel_type.hpp>
#include <common/controller_ctx.hpp>
#include <common/electrical_system.hpp>
#include <common/power_consumption.hpp>
#include <common/subsystem.hpp>
#include <common/task_scheduler/task_scheduler_interface.hpp>

namespace hydroctrl {
namespace common {

/** @brief Hourly reset randomization period
 *
 * With the assumption of a 32 channel relay module and a maximum activation
 * period of 30 min, there is more than enough time available to add random
 * delay. This reduces spikes, as opposed to activating all relays at the same
 * time. Evenly spreading out activations over a period of 3 min minimizes this
 * problem.
 *
 * With emergency (5 min) profile this means that multiple channels may not all
 * fully overlap at the same time. However, this setting is quite radical in the
 * first place and the function of the channels are typically independent within
 * the shorter time span of an hour. This is not seen as a problem.
 */
constexpr std::chrono::milliseconds hourly_reset_randomization_period =
    std::chrono::milliseconds(3 * 60 * 1000);

constexpr int six_in_the_morning = 6;

constexpr int midnight = 0;

//---------------------------------------------------------------------------------------------------------------------

/** Channel */
class channel
{
  public:
    channel(common::subsystem subsystem,
            common::electrical_system electrical_system,
            common::channel_type channel_type,
            std::shared_ptr<common::controller_ctx> ctx);

    /** Get subsystem */
    common::subsystem subsystem();

    /** Electrical system */
    common::electrical_system electrical_system();

    /** Get channel type */
    common::channel_type channel_type();

    /** Get context */
    std::shared_ptr<common::controller_ctx> ctx();

    /** @brief Set allocated relay module index
     *
     * @param idx  Zero based index
     */
    void set_relay_module_idx(int idx);

    /** @brief Set allocated relay module indexes
     *
     * @param indexes  Indexes
     */
    void set_relay_module_idx(std::vector<int> indexes);

    /** Get relay module indexes */
    std::vector<int> relay_indexes();

    /** Set power consumption profile
     *
     * The function invokes activate() to initiate the new power profile
     */
    void
    set_power_consumption_profile(common::power_consumption_profile profile);

    /** Get power consumption profile */
    common::power_consumption_profile power_profile();

    /** @brief Hourly channel tick
     *
     * This method is invoked for all channels when the hourly transition
     * occurs.
     *
     * This triggers a delayed channel activation, where channel refresh is
     * randomly distributed time. This reduces the electrical spikes of
     * inductive or non-inductive devices that are power cycled close in time.
     *
     * The random start period also affects the power schedule trigger point
     * when each channel is being disabled. For example, two channels with
     * identical power profile (e.g. 30 min), stop at different times since the
     * timer is started differently.
     */
    virtual void hourly_tick();

    /** Physical activation
     *
     * This method is immediately activating the channel. It is typically
     * invoked from a context where constraints exists. However, it can be used
     * when testing during development
     */
    virtual void activate() = 0;

    /** Physical deactivation
     *
     * This method is immediately deactivating the channel. It is typically
     * invoked from a context where constraints exists. However, it can be used
     * when testing during development
     */
    virtual void deactivate() = 0;

    /** Determine activation duration */
    std::chrono::milliseconds
    determine_activation_duration(common::power_consumption_profile profile);

    /** Calculate new randomized delay in milliseconds */
    std::chrono::milliseconds random_start_delay();

  private:
    /** Calculate hourly milliseconds remaining in the current hour */
    std::chrono::milliseconds hourly_remaining();

    /** Calculate duration of hourly power profile */
    std::chrono::milliseconds
    duration_of_hourly_power_profile(common::power_consumption_profile profile);

    /** Power profile */
    common::power_consumption_profile power_profile_{
        common::power_consumption_profile::off};

    /** Sub system */
    common::subsystem subsystem_;

    /** Electrical system */
    common::electrical_system electrical_system_;

    /** Channel type */
    common::channel_type channel_type_;

    /** Controller context */
    std::shared_ptr<common::controller_ctx> ctx_;

    /** Relay indexes */
    std::vector<int> relay_indexes_;
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
