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

#include <common/channel/channel.hpp>
#include <common/channel/subsystem/core/rotary_alarm_light_channel.hpp>
#include <common/channel/subsystem/main/full_spectrum_light_channel.hpp>
#include <common/channel/subsystem/main/step_down_voltage_transformer_channel.hpp>
#include <common/channel/subsystem/main/ventilation_fan_channel.hpp>
#include <common/channel/subsystem/main/water_pump_channel.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

/** Channel collection */
struct channel_collection
{
    /******************************************************************************************
     ********************************** AC 220-240 VOLT
     ****************************************
     *****************************************************************************************/

    /** Rotary alarm light channel */
    std::shared_ptr<rotary_alarm_light_channel> rotary_alarm_light;

    /** Ventilation fan channel */
    std::shared_ptr<ventilation_fan_channel> ventilation_fan;

    /** Upper full spectrum light channel */
    std::shared_ptr<full_spectrum_light_channel> upper_full_spectrum_light;

    /** Lower full spectrum light channel */
    std::shared_ptr<full_spectrum_light_channel> lower_full_spectrum_light;

    /** Lower water pump #1 */
    std::shared_ptr<water_pump_channel> lower_water_pump_1;

    /** Lower water pump #2 */
    std::shared_ptr<water_pump_channel> lower_water_pump_2;

    /** Lower water pump #3 */
    std::shared_ptr<water_pump_channel> lower_water_pump_3;

    /** Step down voltage transformer */
    std::shared_ptr<step_down_voltage_transformer_channel>
        step_down_voltage_transformer;

    /******************************************************************************************
     ********************************** AC 110-120 VOLT
     ****************************************
     *****************************************************************************************/

    /** Upper water pump #1 */
    std::shared_ptr<water_pump_channel> upper_water_pump_1;

    /** Upper water pump #2 */
    std::shared_ptr<water_pump_channel> upper_water_pump_2;

    /******************************************************************************************
     ************************************* COMMON
     **********************************************
     *****************************************************************************************/

    /** Iterator friendly vector */
    std::vector<std::shared_ptr<channel>> all_channels;
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
