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
#include <common/ventilation_fan.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

class ventilation_fan_channel : public channel
{
  public:
    /** Constructor */
    ventilation_fan_channel(common::electrical_system electrical_system,
                            std::shared_ptr<common::controller_ctx> ctx);

    /** Set fan mode */
    void set_fan_mode(common::ventilation_fan_mode mode);

    /** Get fan mode */
    common::ventilation_fan_mode fan_mode();

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
    /** Channel update needed */
    bool channel_update_needed();

    /** Channel activation timer callback */
    static void channel_activation_cb(common::task_context task_ctx,
                                      ventilation_fan_channel *_this);

    /** Channel deactivation timer callback */
    static void channel_deactivation_cb(common::task_context task_ctx,
                                        ventilation_fan_channel *_this);

    /** Safe relay activation */
    static void safe_relay_activation(ventilation_fan_channel *_this);

    /** Fan mode */
    common::ventilation_fan_mode fan_mode_{
        common::ventilation_fan_mode::automatic};

    /** Fan RPM setting */
    common::ventilation_fan_mode fan_rpm_setting_{
        common::ventilation_fan_mode::none};

    /** All but the latest deactivation timers are ignored */
    hydroctrl::common::task_id latest_deactivation_task_id_{0};
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
