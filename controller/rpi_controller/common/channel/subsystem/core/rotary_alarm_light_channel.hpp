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

class rotary_alarm_light_channel : public channel
{
  public:
    explicit rotary_alarm_light_channel(
        std::shared_ptr<common::controller_ctx> ctx);

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
    bool activated_{false};
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
