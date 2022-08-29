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

#include <CppLinuxSerial/SerialPort.hpp>

using namespace mn::CppLinuxSerial;

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

    request_handler_ =
        std::make_shared<user_interface::request_handler>(cfg, ctx_);

    channel_collection_ = std::make_shared<common::channel_collection>();

    /******************************************************************************************/
    /*********************************** AC 220-240 VOLT **************************************/
    /******************************************************************************************/

    auto ch_a0 = std::make_shared<common::rotary_alarm_light_channel>(
        common::electrical_system::ac_220_240_volt, ctx_);
    ch_a0->set_relay_module_idx(
        common::relay_channel_idx_rotary_alarm_light_channel);
    channel_collection_->rotary_alarm_light = ch_a0;
    channel_collection_->all_channels.emplace_back(ch_a0);

    auto ch_a1 = std::make_shared<common::ventilation_fan_channel>(
        common::electrical_system::ac_220_240_volt, ctx_);
    std::vector<int> relay_indexes;
    relay_indexes.emplace_back(
        common::relay_channel_idx_ventilation_fan_channel_low_rpm);
    relay_indexes.emplace_back(
        common::relay_channel_idx_ventilation_fan_channel_high_rpm);
    ch_a1->set_relay_module_idx(relay_indexes);
    channel_collection_->ventilation_fan = ch_a1;
    channel_collection_->all_channels.emplace_back(ch_a1);

    auto ch_a2 = std::make_shared<common::full_spectrum_light_channel>(
        common::electrical_system::ac_220_240_volt,
        common::channel_type::upper_full_spectrum_light, ctx_);
    ch_a2->set_relay_module_idx(
        common::relay_channel_idx_upper_full_spectrum_light_channel);
    channel_collection_->upper_full_spectrum_light = ch_a2;
    channel_collection_->all_channels.emplace_back(ch_a2);

    auto ch_a3 = std::make_shared<common::full_spectrum_light_channel>(
        common::electrical_system::ac_220_240_volt,
        common::channel_type::lower_full_spectrum_light, ctx_);
    ch_a3->set_relay_module_idx(
        common::relay_channel_idx_lower_full_spectrum_light_channel);
    channel_collection_->lower_full_spectrum_light = ch_a3;
    channel_collection_->all_channels.emplace_back(ch_a3);

    auto ch_a4 = std::make_shared<common::water_pump_channel>(
        common::electrical_system::ac_220_240_volt,
        common::channel_type::lower_water_pump_1, ctx_);
    ch_a4->set_relay_module_idx(common::relay_channel_idx_lower_water_pump_1);
    channel_collection_->lower_water_pump_1 = ch_a4;
    channel_collection_->all_channels.emplace_back(ch_a4);

    auto ch_a5 = std::make_shared<common::water_pump_channel>(
        common::electrical_system::ac_220_240_volt,
        common::channel_type::lower_water_pump_2, ctx_);
    ch_a5->set_relay_module_idx(common::relay_channel_idx_lower_water_pump_2);
    channel_collection_->lower_water_pump_2 = ch_a5;
    channel_collection_->all_channels.emplace_back(ch_a5);

    auto ch_a6 = std::make_shared<common::water_pump_channel>(
        common::electrical_system::ac_220_240_volt,
        common::channel_type::lower_water_pump_3, ctx_);
    ch_a6->set_relay_module_idx(common::relay_channel_idx_lower_water_pump_3);
    channel_collection_->lower_water_pump_3 = ch_a6;
    channel_collection_->all_channels.emplace_back(ch_a6);

    auto ch_a7 =
        std::make_shared<common::step_down_voltage_transformer_channel>(
            common::electrical_system::ac_220_240_volt,
            common::channel_type::step_down_voltage_transformer, ctx_);
    ch_a7->set_relay_module_idx(
        common::relay_channel_idx_step_down_voltage_transformer);
    channel_collection_->step_down_voltage_transformer = ch_a7;
    channel_collection_->all_channels.emplace_back(ch_a7);

    /******************************************************************************************/
    /*********************************** AC 110-120 VOLT **************************************/
    /******************************************************************************************/

    auto ch_b0 = std::make_shared<common::water_pump_channel>(
        common::electrical_system::ac_110_120_volt,
        common::channel_type::upper_water_pump_1, ctx_);
    ch_b0->set_relay_module_idx(common::relay_channel_idx_upper_water_pump_1);
    channel_collection_->upper_water_pump_1 = ch_b0;
    channel_collection_->all_channels.emplace_back(ch_b0);

    auto ch_b1 = std::make_shared<common::water_pump_channel>(
        common::electrical_system::ac_110_120_volt,
        common::channel_type::upper_water_pump_2, ctx_);
    ch_b1->set_relay_module_idx(common::relay_channel_idx_upper_water_pump_2);
    channel_collection_->upper_water_pump_2 = ch_b1;
    channel_collection_->all_channels.emplace_back(ch_b1);
}

//---------------------------------------------------------------------------------------------------------------------

void controller::refresh_transformer_state(controller *_this)
{
    auto channel_collection = _this->channel_collection_;

    bool transformer_required = false;
    for (auto &&ch : channel_collection->all_channels) {
        auto power_profile = ch->power_profile();
        auto electrical_system = ch->electrical_system();

        if (electrical_system == common::electrical_system::ac_110_120_volt &&
            power_profile != common::power_consumption_profile::off) {
            transformer_required = true;
        }
    }

    if (transformer_required) {
        channel_collection->step_down_voltage_transformer->activate();
    } else {
        channel_collection->step_down_voltage_transformer->deactivate();
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

    /******************************************************************************************/
    /*********************************** AC 220-240 VOLT **************************************/
    /******************************************************************************************/

    switch (channel_type) {
    case common::channel_type::rotary_alarm_light:
        channel_collection->rotary_alarm_light->set_power_consumption_profile(
            power_profile);

        if (power_profile == common::power_consumption_profile::off) {
            channel_collection->rotary_alarm_light->deactivate();
        } else {
            channel_collection->rotary_alarm_light->activate();
        }
        break;
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
    case common::channel_type::lower_water_pump_1:
        channel_collection->lower_water_pump_1->set_power_consumption_profile(
            power_profile);

        if (power_profile == common::power_consumption_profile::off) {
            channel_collection->lower_water_pump_1->deactivate();
        } else {
            channel_collection->lower_water_pump_1->activate();
        }
        break;
    case common::channel_type::lower_water_pump_2:
        channel_collection->lower_water_pump_2->set_power_consumption_profile(
            power_profile);

        if (power_profile == common::power_consumption_profile::off) {
            channel_collection->lower_water_pump_2->deactivate();
        } else {
            channel_collection->lower_water_pump_2->activate();
        }
        break;
    case common::channel_type::lower_water_pump_3:
        channel_collection->lower_water_pump_3->set_power_consumption_profile(
            power_profile);

        if (power_profile == common::power_consumption_profile::off) {
            channel_collection->lower_water_pump_3->deactivate();
        } else {
            channel_collection->lower_water_pump_3->activate();
        }
        break;
    default:
        break;
    }

    /******************************************************************************************/
    /*********************************** AC 110-120 VOLT **************************************/
    /******************************************************************************************/

    switch (channel_type) {
    case common::channel_type::upper_water_pump_1:
        channel_collection->upper_water_pump_1->set_power_consumption_profile(
            power_profile);

        refresh_transformer_state(_this);

        if (power_profile == common::power_consumption_profile::off) {
            channel_collection->upper_water_pump_1->deactivate();
        } else {
            channel_collection->upper_water_pump_1->activate();
        }
        break;
    case common::channel_type::upper_water_pump_2:
        channel_collection->upper_water_pump_2->set_power_consumption_profile(
            power_profile);

        refresh_transformer_state(_this);

        if (power_profile == common::power_consumption_profile::off) {
            channel_collection->upper_water_pump_2->deactivate();
        } else {
            channel_collection->upper_water_pump_2->activate();
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
        //  refresh_transformer_state(_this);

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

    // Update system wide alarm status
    auto ctx = _this->ctx_;
    if (ctx->chassi_status.door_alarm) {
        ctx->system_wide_alarm_ = true;
    } else {
        // over time more alarms are seen as critical
        // When using several inputs, this line cannot
        // clear the overall status
        ctx->system_wide_alarm_ = false;
    }

    if (ctx->system_wide_alarm_) {
        channel_collection->rotary_alarm_light->activate();
    } else {
        channel_collection->rotary_alarm_light->deactivate();
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

void controller::serial_console_thread_main(controller *_this)
{
#ifdef HC_SERIAL_CONSOLE_ENABLED

    auto ctx = _this->ctx_;

    SerialPort serialPort("/dev/ttyACM0", BaudRate::B_9600, NumDataBits::EIGHT,
                          Parity::NONE, NumStopBits::ONE);

    // Blocking read
    serialPort.SetTimeout(-1);

    serialPort.Open();

    std::stringstream status;

    while (true) {
        std::string rx;
        serialPort.Read(rx);
        status << rx;

        auto pos = status.str().find("\n");
        if (pos != std::string::npos) {
            auto entry = status.str().substr(0, pos);

            // Ignore initial incomplete entry
            if (entry.size() > 80) {

                // Get stored state (and update below when possible)
                common::chassi_measurements status;
                {
                    const std::lock_guard<std::mutex> lock(*ctx->mutex);
                    status = ctx->chassi_status;
                }

                // Ambient temperature
                auto temp = common::extract_double(entry, ".*temp=(.*?),");
                if (temp > 0 && temp < 100) {
                    status.ambient_temp = temp;
                }

                // Ambient humidity
                auto humidity =
                    common::extract_double(entry, ".*humidity=(.*?),");
                if (humidity > 0 && humidity < 100) {
                    status.ambient_humidity = humidity;
                }

                // Chassi temperature
                auto temp2 = common::extract_double(entry, ".*temp2=(.*?),");
                if (temp2 > 0 && temp2 < 100) {
                    status.chassi_temp = temp2;
                }

                // Chassi humidity
                auto humidity2 =
                    common::extract_double(entry, ".*humidity2=(.*?),");
                if (humidity2 > 0 && humidity2 < 100) {
                    status.chassi_humidity = humidity2;
                }

                // Door alarm
                auto door_alarm =
                    common::extract_integer(entry, ".*door_alarm=(\\d),");
                if (door_alarm == 0) {
                    status.door_alarm = false;
                } else if (door_alarm == 1) {
                    status.door_alarm = true;
                } // -1 if unavailable

                // Chassi temperature warning
                auto chassi_temp_warning = common::extract_integer(
                    entry, ".*chassi_temp_warning=(\\d),");
                if (chassi_temp_warning == 0) {
                    status.chassi_temp_warning = false;
                } else if (chassi_temp_warning == 1) {
                    status.chassi_temp_warning = true;
                } // -1 if unavailable

                // Chassi temperature alarm
                auto chassi_temp_alarm = common::extract_integer(
                    entry, ".*chassi_temp_alarm=(\\d);");
                if (chassi_temp_alarm == 0) {
                    status.chassi_temp_alarm = false;
                } else if (chassi_temp_alarm == 1) {
                    status.chassi_temp_alarm = true;
                } // -1 if unavailable

                std::stringstream log_msg;
                log_msg << "[controller::serial_console_thread_main] "
                        << "entry '" << entry << "'";
                common::log(common::log_level::log_level_debug, log_msg.str());

                log_msg = std::stringstream("");
                log_msg << "[controller::serial_console_thread_main] "
                        << "ambient_temp " << status.ambient_temp << ", "
                        << "ambient_humidity " << status.ambient_humidity
                        << ", "
                        << "chassi_temp " << status.chassi_temp << ", "
                        << "chassi_humidity " << status.chassi_humidity << ", "
                        << "door_alarm " << status.door_alarm << " "
                        << "chassi_temp_warning " << status.chassi_temp_warning
                        << ", "
                        << "chassi_temp_alarm " << status.chassi_temp_alarm
                        << std::endl;
                common::log(common::log_level::log_level_info, log_msg.str());

                // Update shared structure
                {
                    const std::lock_guard<std::mutex> lock(*ctx->mutex);
                    ctx->chassi_status = status;
                }
            }

            auto remaining = status.str().substr(pos, status.str().size() - 1);
            status = std::stringstream(remaining);
        }
    }

    serialPort.Close();
#else  // HC_SERIAL_CONSOLE_ENABLED
    common::log(common::log_level::log_level_notice,
                "[controller::serial_console_thread_main] stubbed\n");
#endif // HC_SERIAL_CONSOLE_ENABLED
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
    auto serial_console_thread = std::thread(serial_console_thread_main, this);

    // Wait for threads exit
    task_scheduler_thread.join();
    socket_user_interface_thread.join();
    serial_console_thread.join();
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace controller
} // namespace hydroctrl
