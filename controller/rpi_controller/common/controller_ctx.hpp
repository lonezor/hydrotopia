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

#include <common/configuration.hpp>
#include <common/relay_module/relay_module.hpp>
#include <common/system_clock.hpp>
#include <common/task_scheduler/task_scheduler_interface.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

struct controller_ctx
{
    std::shared_ptr<common::configuration> config{nullptr};
    std::shared_ptr<common::task_scheduler_interface> task_scheduler{nullptr};
    std::shared_ptr<common::system_clock> clock{nullptr};
    std::shared_ptr<common::relay_module> relay_module{nullptr};
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
