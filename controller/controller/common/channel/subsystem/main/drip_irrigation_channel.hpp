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

#include <chrono>

#include <common/channel/channel.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

class drip_irrigation_channel : public channel
{
  public:
    /** Constructor */
    drip_irrigation_channel(common::channel_type channel_type,
                            std::shared_ptr<common::controller_ctx> ctx);

    /** @brief Hourly tick
     *
     * The hourly tick is used to regulate activation:
     *  - Activation at certain hour of the day (06:00)
     *
     * The hourly tick is used to regulate deactivation:
     *  - Deactivation after power profile duration has been reached
     */
    void hourly_tick() final;

    /** @brief Activate channel
     *
     * Immediate channel activation.
     */
    void activate() final;

    /** @brief Deactivate channel
     *
     * Immediate channel deactivation
     */
    void deactivate() final;

  private:
    /** Calculate duration of daily power profile */
    std::chrono::hours
    duration_of_daily_power_profile(common::power_consumption_profile profile);

    /** Hours activated */
    std::chrono::hours hours_activated_{std::chrono::hours(0)};

    /** Activated */
    bool activated_{false};
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
