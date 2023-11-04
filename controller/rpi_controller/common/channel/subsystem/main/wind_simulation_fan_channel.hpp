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

#include <common/channel/channel.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

/** Water pump channel */
class wind_simulation_fan_channel : public channel
{
  public:
    wind_simulation_fan_channel(common::electrical_system electrical_system,
                                common::channel_type channel_type,
                                std::shared_ptr<common::controller_ctx> ctx);

    /** @brief Hourly tick
     *
     * This triggers delayed activation ending up calling activate()
     */
    void hourly_tick() final;

    /** @brief Activate channel
     *
     * Immediate channel activation.
     *
     * Channel deactivation timer is configured when applicable
     */
    void activate() final;

    /** @brief Deactivate channel
     *
     * Immediate channel deactivation
     */
    void deactivate() final;

  private:
    /** Channel activation timer callback */
    static void channel_activation_cb(common::task_context task_ctx,
                                      wind_simulation_fan_channel *_this);

    /** Channel deactivation timer callback */
    static void channel_deactivation_cb(common::task_context task_ctx,
                                        wind_simulation_fan_channel *_this);

    /** All but the latest deactivation timers are ignored */
    hydroctrl::common::task_id latest_deactivation_task_id_{0};

  private:
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
