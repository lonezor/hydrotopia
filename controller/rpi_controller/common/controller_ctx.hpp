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

#include <functional>
#include <memory>
#include <mutex>

#include <common/channel_type.hpp>
#include <common/configuration.hpp>
#include <common/power_consumption.hpp>
#include <common/relay_module/relay_module.hpp>
#include <common/system_clock.hpp>
#include <common/task_scheduler/task_scheduler_interface.hpp>
#include <common/ventilation_fan.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

/** Chassi measurements */
struct chassi_measurements
{
    double ambient_temp{0};
    double ambient_humidity{0};
    double chassi_temp{0};
    double chassi_humidity{0};
    bool door_alarm{false};
    bool chassi_temp_warning{false};
    bool chassi_temp_alarm{false};
};

//---------------------------------------------------------------------------------------------------------------------

/** Controller context */
struct controller_ctx
{
    std::shared_ptr<std::mutex> mutex{nullptr};
    std::shared_ptr<common::configuration> config{nullptr};
    std::shared_ptr<common::task_scheduler_interface> task_scheduler{nullptr};
    std::shared_ptr<common::system_clock> clock{nullptr};
    std::shared_ptr<common::relay_module> relay_module{nullptr};
    chassi_measurements chassi_status;
    bool system_wide_alarm_{false};

    /** @brief Callback function: user_request_set_ventilation_fan_mode()
     *
     * @param fan_mode  Fan mode
     */
    std::shared_ptr<std::function<void(common::ventilation_fan_mode)>>
        user_request_set_ventilation_fan_mode{nullptr};

    /** @brief Callback function: user_request_set_power_mode()
     *
     * @param channel_type   Channel type
     * @param power_profile  Power profile
     */
    std::shared_ptr<std::function<void(common::channel_type,
                                       common::power_consumption_profile)>>
        user_request_set_power_mode{nullptr};
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
