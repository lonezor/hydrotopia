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

#include <thread>

#include <common/channel_index.hpp>
#include <common/string_processing/regex.hpp>
#include <controller/controller.hpp>
#include <user_interface/socket_user_interface/request_handler.hpp>

namespace hydroctrl {
namespace controller {

//---------------------------------------------------------------------------------------------------------------------

controller::controller(std::shared_ptr<common::configuration> cfg)
{
    /******************************************************************************************/
    /*********************************** CONTEXT **********************************************/
    /******************************************************************************************/
    ctx_ = std::make_shared<common::controller_ctx>();
    ctx_->mutex = std::make_shared<std::mutex>();
    ctx_->config = cfg;
    ctx_->clock = std::make_shared<common::system_clock>();
    ctx_->relay_module = std::make_shared<common::relay_module>();

    ctx_->task_scheduler = common::create_task_scheduler();
    ctx_->task_scheduler->set_mutex(ctx_->mutex);

    auto f1 = std::bind(&user_request_set_ventilation_fan_mode,
                        std::placeholders::_1, this);
    ctx_->user_request_set_ventilation_fan_mode =
        std::make_shared<std::function<void(common::ventilation_fan_mode)>>(f1);

    auto f2 = std::bind(&user_request_set_power_profile, std::placeholders::_1,
                        std::placeholders::_2, this);
    ctx_->user_request_set_power_mode = std::make_shared<
        std::function<void(common::channel_type channel_type,
                           common::power_consumption_profile power_profile)>>(
        f2);

    auto f3 =
        std::bind(&user_request_development_cmd, std::placeholders::_1, this);
    ctx_->user_request_development_cmd =
        std::make_shared<std::function<void(std::string)>>(f3);

    request_handler_ =
        std::make_shared<user_interface::request_handler>(cfg, ctx_);

    channel_collection_ = std::make_shared<common::channel_collection>();

    auto ch_a0 = std::make_shared<common::ventilation_fan_channel>(ctx_);
    // At the moment, rpm modes are disabled; vector has two identical entries
    std::vector<int> relay_indexes;
    relay_indexes.emplace_back(
        common::relay_channel_idx_ventilation_fan_channel_high_rpm);
    relay_indexes.emplace_back(
        common::relay_channel_idx_ventilation_fan_channel_high_rpm);
    ch_a0->set_relay_module_idx(relay_indexes);
    channel_collection_->ventilation_fan = ch_a0;
    channel_collection_->all_channels.emplace_back(ch_a0);

    auto ch_a1 = std::make_shared<common::full_spectrum_light_channel>(
        common::channel_type::upper_full_spectrum_light, ctx_);
    ch_a1->set_relay_module_idx(
        common::relay_channel_idx_upper_full_spectrum_light_channel);
    channel_collection_->upper_full_spectrum_light = ch_a1;
    channel_collection_->all_channels.emplace_back(ch_a1);

    auto ch_a2 = std::make_shared<common::full_spectrum_light_channel>(
        common::channel_type::lower_full_spectrum_light, ctx_);
    ch_a2->set_relay_module_idx(
        common::relay_channel_idx_lower_full_spectrum_light_channel);
    channel_collection_->lower_full_spectrum_light = ch_a2;
    channel_collection_->all_channels.emplace_back(ch_a2);

    auto ch_a3 = std::make_shared<common::wind_simulation_fan_channel>(
        common::channel_type::wind_simulation_fan, ctx_);
    ch_a3->set_relay_module_idx(
        common::relay_channel_idx_wind_simulation_fan_channel);
    channel_collection_->wind_simulation_fan = ch_a3;
    channel_collection_->all_channels.emplace_back(ch_a3);

    auto ch_a4 = std::make_shared<common::drip_irrigation_channel>(
        common::channel_type::drip_irrigation, ctx_);
    ch_a4->set_relay_module_idx(
        common::relay_channel_idx_drip_irrigation_channel);
    channel_collection_->drip_irrigation = ch_a4;
    channel_collection_->all_channels.emplace_back(ch_a4);
}

//---------------------------------------------------------------------------------------------------------------------

void controller::refresh_transformer_state(controller *_this)
{
    auto channel_collection = _this->channel_collection_;

}

//---------------------------------------------------------------------------------------------------------------------

void controller::user_request_development_cmd(std::string cmd,
                                              controller *_this)
{
    auto ctx = _this->ctx_;

    if (cmd.find("development_cmd_cabinet_temperature_low") !=
        std::string::npos) {
        ctx->cabinet_status.cabinet_temperature =
            common::temperature(common::temperature::temp_unit::celcius, 20);
    }

    else if (cmd.find("development_cmd_cabinet_temperature_high") !=
             std::string::npos) {
        ctx->cabinet_status.cabinet_temperature =
            common::temperature(common::temperature::temp_unit::celcius, 43);
    }
}

//---------------------------------------------------------------------------------------------------------------------

void controller::user_request_set_ventilation_fan_mode(
    common::ventilation_fan_mode fan_mode, controller *_this)
{
    auto channel_collection = _this->channel_collection_;
    channel_collection->ventilation_fan->set_fan_mode(fan_mode);
}

//---------------------------------------------------------------------------------------------------------------------

void controller::user_request_set_power_profile(
    common::channel_type channel_type,
    common::power_consumption_profile power_profile, controller *_this)
{
    auto channel_collection = _this->channel_collection_;

    switch (channel_type) {
    case common::channel_type::ventilation_fan:
        channel_collection->ventilation_fan->set_power_consumption_profile(
            power_profile);

        if (power_profile == common::power_consumption_profile::off) {
            channel_collection->ventilation_fan->deactivate();
        } else {
            channel_collection->ventilation_fan->activate();
        }
        break;
    case common::channel_type::upper_full_spectrum_light:
        channel_collection->upper_full_spectrum_light
            ->set_power_consumption_profile(power_profile);

        if (power_profile == common::power_consumption_profile::off) {
            channel_collection->upper_full_spectrum_light->deactivate();
        } else {
            channel_collection->upper_full_spectrum_light->activate();
        }
        break;
    case common::channel_type::lower_full_spectrum_light:
        channel_collection->lower_full_spectrum_light
            ->set_power_consumption_profile(power_profile);

        if (power_profile == common::power_consumption_profile::off) {
            channel_collection->lower_full_spectrum_light->deactivate();
        } else {
            channel_collection->lower_full_spectrum_light->activate();
        }
        break;
    case common::channel_type::wind_simulation_fan:
        channel_collection->wind_simulation_fan->set_power_consumption_profile(
            power_profile);

        if (power_profile == common::power_consumption_profile::off) {
            channel_collection->wind_simulation_fan->deactivate();
        } else {
            channel_collection->wind_simulation_fan->activate();
        }
        break;
    case common::channel_type::drip_irrigation:
        channel_collection->drip_irrigation->set_power_consumption_profile(
            power_profile);

        if (power_profile == common::power_consumption_profile::off) {
            channel_collection->drip_irrigation->deactivate();
        } else {
            channel_collection->drip_irrigation->activate();
        }
        break;
    default:
        break;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void controller::system_clock_tick_cb(common::task_context task_ctx,
                                      controller *_this)
{
    auto clock = _this->ctx_->clock;
    auto channel_collection = _this->channel_collection_;

    clock->tick();

    // New hour trigger point
    if (clock->hour_transition()) {
        // Enable transformer when required
        refresh_transformer_state(_this);

        // Hourly tick
        for (auto &&ch : channel_collection->all_channels) {
            ch->hourly_tick();
        }
    }

    // New minute trigger point
    if (clock->minute_transition()) {
        // Update graphical user interface
        auto time = clock->time();
    }
}

//---------------------------------------------------------------------------------------------------------------------

void controller::task_scheduler_thread_main(controller *_this)
{
    auto task_scheduler = _this->ctx_->task_scheduler;
    task_scheduler->run(common::task_scheduler_mode::foreground);
}

//---------------------------------------------------------------------------------------------------------------------

void controller::socket_user_interface_thread_main(controller *_this)
{
    auto request_handler = _this->request_handler_;
    request_handler->run();
}

//---------------------------------------------------------------------------------------------------------------------

void controller::run()
{
    // Setup system clock timer
    auto bf = std::bind(&system_clock_tick_cb, std::placeholders::_1, this);
    auto tid = ctx_->task_scheduler->register_periodic_task(
        std::chrono::milliseconds(500), bf);

    // Setup threads
    auto task_scheduler_thread = std::thread(task_scheduler_thread_main, this);
    auto socket_user_interface_thread =
        std::thread(socket_user_interface_thread_main, this);

    // Wait for threads exit
    task_scheduler_thread.join();
    socket_user_interface_thread.join();
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace controller
} // namespace hydroctrl
