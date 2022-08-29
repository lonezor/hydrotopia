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

#include <common/channel_collection.hpp>
#include <common/configuration.hpp>
#include <common/controller_ctx.hpp>
#include <user_interface/socket_user_interface/request_handler.hpp>

namespace hydroctrl {
namespace controller {

//---------------------------------------------------------------------------------------------------------------------

class controller
{
  public:
    controller(std::shared_ptr<common::configuration> cfg);

    void run();

  private:
    static void system_clock_tick_cb(common::task_context task_ctx,
                                     controller *_this);

    static void task_scheduler_thread_main(controller *_this);

    static void socket_user_interface_thread_main(controller *_this);

    static void serial_console_thread_main(controller *_this);

    static void refresh_transformer_state(controller *_this);

    static void
    user_request_set_ventilation_fan_mode(common::ventilation_fan_mode fan_mode,
                                          controller *_this);

    static void user_request_set_power_profile(
        common::channel_type channel_type,
        common::power_consumption_profile power_profile, controller *_this);

    static void user_request_development_cmd(std::string cmd,
                                             controller *_this);

    std::shared_ptr<common::controller_ctx> ctx_{nullptr};

    std::shared_ptr<common::channel_collection> channel_collection_{nullptr};

    std::shared_ptr<user_interface::request_handler> request_handler_{nullptr};
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace controller
} // namespace hydroctrl
