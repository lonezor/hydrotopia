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
#include <common/channel/subsystem/main/full_spectrum_light_channel.hpp>
#include <common/channel/subsystem/main/ventilation_fan_channel.hpp>
#include <common/channel/subsystem/main/wind_simulation_fan_channel.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

/** Channel collection */
struct channel_collection
{
    /** Ventilation fan channel */
    std::shared_ptr<ventilation_fan_channel> ventilation_fan;

    /** Upper full spectrum light channel */
    std::shared_ptr<full_spectrum_light_channel> upper_full_spectrum_light;

    /** Lower full spectrum light channel */
    std::shared_ptr<full_spectrum_light_channel> lower_full_spectrum_light;

    /** Wind simulation channel */
    std::shared_ptr<wind_simulation_fan_channel> wind_simulation_fan;

    /** Iterator friendly vector */
    std::vector<std::shared_ptr<channel>> all_channels;
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
