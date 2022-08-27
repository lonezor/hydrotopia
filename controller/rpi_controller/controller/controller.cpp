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

#include <controller/controller.hpp>
#include <user_interface/socket_user_interface/request_handler.hpp>

namespace hydroctrl {
namespace controller {

//---------------------------------------------------------------------------------------------------------------------

controller::controller(std::shared_ptr<common::configuration> cfg)
{
    ctx_ = std::make_shared<common::controller_ctx>();
    ctx_->config = cfg;
    ctx_->task_scheduler = common::create_task_scheduler();
    ctx_->clock = std::make_shared<common::system_clock>();
    ctx_->relay_module = std::make_shared<common::relay_module>();

    channel_collection_ = std::make_shared<common::channel_collection>();

    auto e1 = std::make_shared<common::rotary_alarm_light_channel>(ctx_);
    e1->set_relay_module_idx(
        common::relay_channel_idx_rotary_alarm_light_channel);
    channel_collection_->rotary_alarm_light = e1;
    channel_collection_->all_channels.emplace_back(e1);

    auto e2 = std::make_shared<common::ventilation_fan_channel>(ctx_);
    std::vector<int> relay_indexes;
    relay_indexes.emplace_back(
        common::relay_channel_idx_ventilation_fan_channel_low_rpm);
    relay_indexes.emplace_back(
        common::relay_channel_idx_ventilation_fan_channel_high_rpm);
    e2->set_relay_module_idx(relay_indexes);
    channel_collection_->ventilation_fan = e2;
    channel_collection_->all_channels.emplace_back(e2);

    auto e3 = std::make_shared<common::full_spectrum_light_channel>(
        common::channel_type::upper_full_spectrum_light, ctx_);
    e3->set_relay_module_idx(
        common::relay_channel_idx_upper_full_spectrum_light_channel);
    channel_collection_->upper_full_spectrum_light = e3;
    channel_collection_->all_channels.emplace_back(e3);

    auto e4 = std::make_shared<common::full_spectrum_light_channel>(
        common::channel_type::lower_full_spectrum_light, ctx_);
    e4->set_relay_module_idx(
        common::relay_channel_idx_lower_full_spectrum_light_channel);
    channel_collection_->lower_full_spectrum_light = e4;
    channel_collection_->all_channels.emplace_back(e4);

    auto e5 = std::make_shared<common::water_pump_channel>(
        common::channel_type::upper_water_pump_1, ctx_);
    e5->set_relay_module_idx(common::relay_channel_idx_upper_water_pump_1);
    channel_collection_->upper_water_pump_1 = e5;
    channel_collection_->all_channels.emplace_back(e5);

    auto e6 = std::make_shared<common::water_pump_channel>(
        common::channel_type::upper_water_pump_2, ctx_);
    e6->set_relay_module_idx(common::relay_channel_idx_upper_water_pump_2);
    channel_collection_->upper_water_pump_2 = e6;
    channel_collection_->all_channels.emplace_back(e6);

    auto e7 = std::make_shared<common::water_pump_channel>(
        common::channel_type::lower_water_pump_1, ctx_);
    e7->set_relay_module_idx(common::relay_channel_idx_lower_water_pump_1);
    channel_collection_->lower_water_pump_1 = e7;
    channel_collection_->all_channels.emplace_back(e7);

    auto e8 = std::make_shared<common::water_pump_channel>(
        common::channel_type::lower_water_pump_2, ctx_);
    e8->set_relay_module_idx(common::relay_channel_idx_lower_water_pump_2);
    channel_collection_->lower_water_pump_2 = e8;
    channel_collection_->all_channels.emplace_back(e8);

    auto e9 = std::make_shared<common::water_pump_channel>(
        common::channel_type::lower_water_pump_3, ctx_);
    e9->set_relay_module_idx(common::relay_channel_idx_lower_water_pump_3);
    channel_collection_->lower_water_pump_3 = e9;
    channel_collection_->all_channels.emplace_back(e9);

    request_handler_ = std::make_shared<user_interface::request_handler>(
        cfg, channel_collection_);
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
