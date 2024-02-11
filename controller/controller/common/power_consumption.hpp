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
#include <string>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

/** @brief Power consumption profile
 *
 * Controls power consumption for all subsystems beyond the core. Each channel
 * is individually controlled and may have different settings.
 */
enum class power_consumption_profile
{
    /** Initializer value */
    unknown,

    /** @brief Continuous: No actions taken to reduce power consumption
     *
     * Maximum performance. System running with optimal settings */
    continuous,

    /** @brief High: First level of power reduction actions
     *
     * Maximum performance. System running with optimal settings */
    high,

    /** @brief Medium: Second level of power reduction actions */
    medium,

    /** @brief Low: Third level of power reduction actions */
    low,

    /** @brief Emergency: Forth level of power reduction actions
     *
     * Running at very low settings, minimum performance in a situation where
     * power availability is critical */
    emergency,

    /** Intentional power off */
    off,
};

//---------------------------------------------------------------------------------------------------------------------

/** Power consumption profile string */
std::string power_consumption_profile_str(power_consumption_profile profile);

//---------------------------------------------------------------------------------------------------------------------

/** High light power consumption profile: daytime 18h continuous operation */
constexpr std::chrono::hours light_power_consumption_profile_high =
    std::chrono::hours(18);

/** Medium  light power consumption profile: daytime 12h continuous operation */
constexpr std::chrono::hours light_power_consumption_profile_medium =
    std::chrono::hours(12);

/** Low light power consumption profile: daytime 6h continuous operation */
constexpr std::chrono::hours light_power_consumption_profile_low =
    std::chrono::hours(6);

/** Emergency light power consumption profile: daytime 3h continuous operation
 */
constexpr std::chrono::hours light_power_consumption_profile_emergency =
    std::chrono::hours(3);

//---------------------------------------------------------------------------------------------------------------------

/** Continuous hourly power consumption profile: Full 60 min operation per hour
 * (daytime continuous operation) */
constexpr std::chrono::seconds hourly_power_consumption_profile_continuous =
    std::chrono::seconds(60 * 60);

/** High hourly power consumption profile: 45 min operation per hour */
constexpr std::chrono::seconds hourly_power_consumption_profile_high =
    std::chrono::seconds(60 * 45);

/** Medium hourly power consumption profile: 30 min operation per hour */
constexpr std::chrono::seconds hourly_power_consumption_profile_medium =
    std::chrono::seconds(60 * 30);

/** Low hourly power consumption profile: 15 min operation per hour */
constexpr std::chrono::seconds hourly_power_consumption_profile_low =
    std::chrono::seconds(60 * 15);

/** Emergency hourly power consumption profile: 5 min operation per hour */
constexpr std::chrono::seconds hourly_power_consumption_profile_emergency =
    std::chrono::seconds(60 * 5);

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
