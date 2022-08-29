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

#include <sstream>
#include <thread>

#include <common/channel/subsystem/main/step_down_voltage_transformer_channel.hpp>
#include <common/log.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

step_down_voltage_transformer_channel::step_down_voltage_transformer_channel(
    common::electrical_system electrical_system,
    common::channel_type channel_type,
    std::shared_ptr<common::controller_ctx> ctx)
    : channel::channel(common::subsystem::main, electrical_system,
                       common::channel_type::step_down_voltage_transformer, ctx)
{
}

//---------------------------------------------------------------------------------------------------------------------

void step_down_voltage_transformer_channel::activate()
{
    if (activated_) {
        return;
    }

    common::log(common::log_level::log_level_debug,
                "[step_down_voltage_transformer_channel::activate]");

    // activate relay
    auto indexes = relay_indexes();
    if (indexes.size() == 1) {
        ctx()->relay_module->activate(indexes.at(0));
    } else {
        throw std::runtime_error(
            "[step_down_voltage_transformer_channel::activate] critical error");
    }

    // Ensure the transformer is fully online before allowing other dependent
    // channels to be activated
    std::this_thread::sleep_for(std::chrono::seconds(5));

    activated_ = true;
}

//---------------------------------------------------------------------------------------------------------------------

void step_down_voltage_transformer_channel::deactivate()
{
    if (!activated_) {
        return;
    }

    common::log(common::log_level::log_level_debug,
                "[step_down_voltage_transformer_channel::deactivate]");

    // deactivate relay
    auto indexes = relay_indexes();
    if (indexes.size() == 1) {
        ctx()->relay_module->deactivate(indexes.at(0));
    } else {
        throw std::runtime_error("[step_down_voltage_transformer_channel::"
                                 "deactivate] critical error");
    }

    activated_ = false;
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
