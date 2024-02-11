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

#include <algorithm>
#include <endian.h>
#include <iomanip>
#include <memory>
#include <regex>
#include <vector>

#include <common/error_code.hpp>
#include <common/io_monitor.hpp>
#include <common/log.hpp>
#include <common/system/system.hpp>
#include <common/system_clock.hpp>

#include <user_interface/socket_user_interface/request_handler.hpp>

namespace hydroctrl {
namespace user_interface {

//-------------------------------------------------------------------------------------------------------------------

request_handler::request_handler(std::shared_ptr<common::configuration> config,
                                 std::shared_ptr<common::controller_ctx> ctx)
    : config_(config), ctx_(ctx)
{
    io_monitor_ = std::make_shared<common::io_monitor>(common::io_monitor());

    auto sock = create_listening_socket(config_->request_handling_port);

    io_monitor_->register_service_socket(sock);
}

//-------------------------------------------------------------------------------------------------------------------

std::shared_ptr<common::socket>
request_handler::create_listening_socket(int port)
{
    auto listening_socket = std::make_shared<common::socket>(
        common::socket(AF_INET, SOCK_STREAM, 0));

    // Ensure that fork() and exec() combination does not inherit the file
    // descriptor which may prevent the server to bind the socket
    int res =
        common::system::fcntl(listening_socket->get_fd(), F_SETFD, FD_CLOEXEC);
    if (res == -1) {
        throw std::system_error(
            errno, std::system_category(),
            "[request_handler::create_listening_socket] Failed to set "
            "FD_CLOEXEC sockopt");
    }

    // Leave TIME_WAIT in case of a crash and allow it to bind it immediately
    int yes = 1;
    res = common::system::setsockopt(listening_socket->get_fd(), SOL_SOCKET,
                                     SO_REUSEADDR, &yes, sizeof(int));
    if (res == -1) {
        throw std::system_error(
            errno, std::system_category(),
            "[request_handler::create_listening_socket] Failed to set "
            "SO_REUSEADDR sockopt");
    }

    // Bind to host:port
    struct sockaddr_in addr
    {};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    res = common::system::bind(listening_socket->get_fd(), addr);
    if (res == -1) {
        throw std::system_error(
            errno, std::system_category(),
            "[request_handler::create_listening_socket] bind() failed");
    }

    constexpr int queue_size = 10;
    res = common::system::listen(listening_socket->get_fd(), queue_size);
    if (res == -1) {
        throw std::system_error(
            errno, std::system_category(),
            "[request_handler::create_listening_socket] listen() failed");
    }

    common::log(common::log_level_debug,
                "[request_handler::create_listening_socket] fd " +
                    std::to_string(listening_socket->get_fd()));

    return listening_socket;
}

//-------------------------------------------------------------------------------------------------------------------

void request_handler::run()
{
    common::log(common::log_level_debug, "[request_handler::run]");

    while (true) {
        auto events = io_monitor_->wait_for_events();

        // No action required
        if (events.empty()) {
            continue;
        }

        // The server must be able to handle bad client input
        try {
            for (auto &&event : events) {
                const std::lock_guard<std::mutex> lock(*ctx_->mutex);

                common::event_type event_type = event->get_type();

                // Socket event
                if (event_type == common::event_type::socket) {
                    common::log(common::log_level_debug,
                                "[request_handler::run] socket event");

                    auto socket_event =
                        std::dynamic_pointer_cast<common::socket_event>(event);
                    auto socket = socket_event->get_socket();
                    handle_client_socket_event(socket_event);
                }

                // Timer event
                if (event_type == common::event_type::timer) {
                    common::log(common::log_level_debug,
                                "[request_handler::run] timer event");

                    auto timer_event =
                        std::dynamic_pointer_cast<common::timer_event>(event);
                    auto timer_id = timer_event->get_id();

                    if (client_expiration_map_.find(timer_id) !=
                        client_expiration_map_.end()) {
                        client_teardown(timer_id);

                        client_expiration_map_.erase(timer_id);
                        client_expirations_++;
                    }
                }
            }
        } catch (const std::system_error &ex) {
            continue;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------

void request_handler::handle_client_socket_event(
    const std::shared_ptr<common::socket_event> &socket_event)
{
    common::log(common::log_level_debug,
                "[request_handler::handle_client_socket_event]");

    auto sock = socket_event->get_socket();
    if (sock == nullptr) {
        return;
    }

    auto socket_handle = sock->get_fd();

    constexpr int rx_buff_size = 65535;
    std::vector<uint8_t> rx_buffer;
    rx_buffer.resize(rx_buff_size);
    gsl::span<uint8_t> span_buff(rx_buffer.data(), rx_buffer.size());

    struct sockaddr_in addr4
    {};
    memset(&addr4, 0, sizeof(addr4));

    ssize_t recv_len = 0;
    socklen_t addr_len = sizeof(addr4);
    recv_len = common::system::recvfrom(sock->get_fd(), span_buff, 0, addr4,
                                        &addr_len);
    if (recv_len <= 0) {
        client_teardown(sock);
        return;
    }

    // Initial setup
    if (target_map_.find(socket_handle) == target_map_.end()) {
        client_setup(sock);
    }

    // Switch to ASCII
    std::string msg(span_buff.data(), span_buff.data() + recv_len);

    msg.erase(std::remove(msg.begin(), msg.end(), '\n'), msg.end());

    handle_client_msg(sock, msg);
}

//-------------------------------------------------------------------------------------------------------------------

void request_handler::client_setup(const std::shared_ptr<common::socket> &sock)
{
    auto sock_handle = sock->get_fd();

    // Already present!
    if (target_map_.find(sock_handle) != target_map_.end()) {
        throw std::system_error(
            make_error_code(common::hydroctrl_error::target_already_registered),
            "[request_handler::client_setup] Target already registered!");
    }

    struct sockaddr_in addr
    {};
    memset(&addr, 0, sizeof(addr));
    socklen_t addr_len = sizeof(addr);

    int res = common::system::getsockname(sock_handle, addr, &addr_len);
    if (res == -1) {
        std::string msg =
            "[request_handler::client_setup] getsockname() failed";
        common::log(common::log_level_debug, msg);
        client_teardown(sock);
        throw std::system_error(errno, std::system_category(), msg);
    }

    std::string local_ip = common::system::address_to_string(addr);

    res = common::system::getpeername(sock_handle, addr, &addr_len);
    if (res == -1) {
        std::string msg =
            "[request_handler::client_setup] getpeername() failed";
        common::log(common::log_level_debug, msg);
        client_teardown(sock);
        throw std::system_error(errno, std::system_category(), msg);
    }

    std::string remote_ip = common::system::address_to_string(addr);

    std::stringstream ss;
    ss << "[request_handler::client_setup] "
       << "local_ip " << local_ip << ", remote_ip " << remote_ip;
    common::log(common::log_level_debug, ss.str());

    msg_state state;

    target_map_[sock->get_fd()] = state;

    // Create client session expiration timer to enforce short lived connections

    constexpr int64_t client_session_expiration_timer = 3 * 60;
    auto timeout_secs = std::chrono::seconds(client_session_expiration_timer);
    auto timeout_microsecs =
        std::chrono::duration_cast<std::chrono::microseconds>(timeout_secs);

    auto timer_id = io_monitor_->register_one_shot_timer(timeout_microsecs);

    common::log(common::log_level_debug,
                "[request_handler::client_setup] creating timer " +
                    std::to_string(timer_id));

    client_expiration data;
    data.timer_id = timer_id;
    data.sock = sock;

    client_expiration_map_[timer_id] = data;
}

//-------------------------------------------------------------------------------------------------------------------

void request_handler::client_teardown(
    const std::shared_ptr<common::socket> &sock)
{
    auto sock_handle = sock->get_fd();

    std::stringstream msg;
    msg << "[request_handler::client_teardown 1] fd "
        << std::to_string(sock_handle);
    common::log(common::log_level_debug, msg.str());

    if (sock->is_active()) {
        sock->close();
    }

    // Not registered
    if (target_map_.find(sock_handle) == target_map_.end()) {
        return;
    }

    target_map_.erase(sock_handle);
}

//-------------------------------------------------------------------------------------------------------------------

void request_handler::client_teardown(uint64_t timer_id)
{
    if (client_expiration_map_.find(timer_id) == client_expiration_map_.end()) {
        return;
    }

    std::stringstream msg;
    msg << "[request_handler::client_teardown 2] timer_id "
        << std::to_string(timer_id);
    common::log(common::log_level_debug, msg.str());

    auto context = client_expiration_map_[timer_id];
    auto sock_handle = context.sock->get_fd();

    if (context.sock->is_active()) {
        context.sock->close();
    }

    if (target_map_.find(sock_handle) != target_map_.end()) {
        target_map_.erase(sock_handle);
    }
}

//-------------------------------------------------------------------------------------------------------------------

void request_handler::send_channel_states(const std::shared_ptr<common::socket> &sock)
{
    std::stringstream stats;

    auto clock = ctx_->clock;

    stats << clock->date() << " " << clock->time_full() << std::endl
          << std::endl;

    stats << "state_daily_ch_01_off: " << state_daily_ch_01_off_ << std::endl;
    stats << "state_daily_ch_01_3h: " << state_daily_ch_01_3h_ << std::endl;
    stats << "state_daily_ch_01_6h: " << state_daily_ch_01_6h_ << std::endl;
    stats << "state_daily_ch_01_12h: " << state_daily_ch_01_12h_ << std::endl;
    stats << "state_daily_ch_01_18h: " << state_daily_ch_01_18h_ << std::endl;
    stats << "state_daily_ch_02_off: " << state_daily_ch_02_off_ << std::endl;
    stats << "state_daily_ch_02_3h: " << state_daily_ch_02_3h_ << std::endl;
    stats << "state_daily_ch_02_6h: " << state_daily_ch_02_6h_ << std::endl;
    stats << "state_daily_ch_02_12h: " << state_daily_ch_02_12h_ << std::endl;
    stats << "state_daily_ch_02_18h: " << state_daily_ch_02_18h_ << std::endl;
    stats << "state_hourly_ch_01_off: " << state_hourly_ch_01_off_ << std::endl;
    stats << "state_hourly_ch_01_5m: " << state_hourly_ch_01_5m_ << std::endl;
    stats << "state_hourly_ch_01_15m: " << state_hourly_ch_01_15m_ << std::endl;
    stats << "state_hourly_ch_01_30m: " << state_hourly_ch_01_30m_ << std::endl;
    stats << "state_hourly_ch_01_45m: " << state_hourly_ch_01_45m_ << std::endl;
    stats << "state_hourly_ch_02_5m: " << state_hourly_ch_02_5m_ << std::endl;
    stats << "state_hourly_ch_02_15m: " << state_hourly_ch_02_15m_ << std::endl;
    stats << "state_hourly_ch_02_30m: " << state_hourly_ch_02_30m_ << std::endl;
    stats << "state_hourly_ch_02_45m: " << state_hourly_ch_02_45m_ << std::endl;
    stats << "state_hourly_ch_03_off: " << state_hourly_ch_03_off_ << std::endl;
    stats << "state_hourly_ch_03_5m: " << state_hourly_ch_03_5m_ << std::endl;
    stats << "state_hourly_ch_03_15m: " << state_hourly_ch_03_15m_ << std::endl;
    stats << "state_hourly_ch_03_30m: " << state_hourly_ch_03_30m_ << std::endl;
    stats << "state_hourly_ch_03_45m: " << state_hourly_ch_03_45m_ << std::endl;

    auto tx_buffer = stats.str();

    gsl::span<const char> tx_span(tx_buffer.c_str(), strlen(tx_buffer.c_str()));
    common::system::send(sock->get_fd(), tx_span, 0);
}

//-------------------------------------------------------------------------------------------------------------------

void request_handler::send_stats(const std::shared_ptr<common::socket> &sock)
{
    std::stringstream stats;

    auto clock = ctx_->clock;

    stats << clock->date() << " " << clock->time_full() << std::endl
          << std::endl;
    stats << ctx_->relay_module->stats();

    auto tx_buffer = stats.str();

    gsl::span<const char> tx_span(tx_buffer.c_str(), strlen(tx_buffer.c_str()));
    common::system::send(sock->get_fd(), tx_span, 0);
}

//-------------------------------------------------------------------------------------------------------------------

void request_handler::send_help_screen(
    const std::shared_ptr<common::socket> &sock)
{
    std::stringstream help_screen;

    // Socket user interface: rephrase power profile to time durations
    help_screen
        << "Available commands:" << std::endl
        << " - ventilation_fan_rpm_low" << std::endl
        << " - ventilation_fan_rpm_high" << std::endl
        << " - ventilation_fan_rpm_automatic" << std::endl

        << " - ventilation_fan_power_profile_off" << std::endl
        << " - ventilation_fan_power_profile_hourly_05min" << std::endl
        << " - ventilation_fan_power_profile_hourly_15min" << std::endl
        << " - ventilation_fan_power_profile_hourly_30min" << std::endl
        << " - ventilation_fan_power_profile_hourly_45min" << std::endl
        << " - ventilation_fan_power_profile_hourly_60min" << std::endl

        << " - upper_full_spectrum_light_power_profile_off" << std::endl
        << " - upper_full_spectrum_light_power_profile_daily_03h" << std::endl
        << " - upper_full_spectrum_light_power_profile_daily_06h" << std::endl
        << " - upper_full_spectrum_light_power_profile_daily_12h" << std::endl
        << " - upper_full_spectrum_light_power_profile_daily_18h" << std::endl

        << " - lower_full_spectrum_light_power_profile_off" << std::endl
        << " - lower_full_spectrum_light_power_profile_daily_03h" << std::endl
        << " - lower_full_spectrum_light_power_profile_daily_06h" << std::endl
        << " - lower_full_spectrum_light_power_profile_daily_12h" << std::endl
        << " - lower_full_spectrum_light_power_profile_daily_18h" << std::endl

        << " - wind_simulation_fan_power_profile_off" << std::endl
        << " - wind_simulation_fan_power_profile_hourly_05min" << std::endl
        << " - wind_simulation_fan_power_profile_hourly_15min" << std::endl
        << " - wind_simulation_fan_power_profile_hourly_30min" << std::endl
        << " - wind_simulation_fan_power_profile_hourly_45min" << std::endl
        << " - wind_simulation_fan_power_profile_hourly_60min" << std::endl

        << " - development_cmd" << std::endl
        << " - channel_states" << std::endl
        << " - stats" << std::endl
        << std::endl;

    auto tx_buffer = help_screen.str();

    gsl::span<const char> tx_span(tx_buffer.c_str(), strlen(tx_buffer.c_str()));
    common::system::send(sock->get_fd(), tx_span, 0);
}

//-------------------------------------------------------------------------------------------------------------------

void request_handler::handle_client_msg(
    const std::shared_ptr<common::socket> &sock, const std::string &msg)
{
    std::stringstream log_msg;
    log_msg << "[request_handler::handle_client_msg] msg '" << msg << "'"
            << msg;
    common::log(common::log_level::log_level_debug, log_msg.str());

    /***** Ventilation RPM *****/
    if (msg.find("ventilation_fan_rpm_low") != std::string::npos) {
        (*ctx_->user_request_set_ventilation_fan_mode)(
            common::ventilation_fan_mode::low);
            state_ventilation_fan_rpm_low_ = true;
            state_ventilation_fan_rpm_high_ = false;
    } else if (msg.find("ventilation_fan_rpm_high") != std::string::npos) {
        (*ctx_->user_request_set_ventilation_fan_mode)(
            common::ventilation_fan_mode::high);
        state_ventilation_fan_rpm_low_ = false;
        state_ventilation_fan_rpm_high_ = true;
    } else if (msg.find("ventilation_fan_rpm_automatic") != std::string::npos) {
        (*ctx_->user_request_set_ventilation_fan_mode)(
            common::ventilation_fan_mode::automatic);

        /***** Ventilation Fan Power Profile *****/
    } else if (msg.find("ventilation_fan_power_profile_off") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::ventilation_fan,
            common::power_consumption_profile::off);
            state_hourly_ch_01_off_ = true;
            state_hourly_ch_01_5m_ = false;
            state_hourly_ch_01_15m_ = false;
            state_hourly_ch_01_30m_ = false;
            state_hourly_ch_01_45m_ = false;
            state_hourly_ch_02_5m_ = false;
            state_hourly_ch_02_15m_ = false;
            state_hourly_ch_02_30m_ = false;
            state_hourly_ch_02_45m_ = false;
    } else if (msg.find("ventilation_fan_power_profile_hourly_05min") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::ventilation_fan,
            common::power_consumption_profile::emergency);
            // mutually exclusive mode
            if (state_ventilation_fan_rpm_high_) {
                state_hourly_ch_01_off_ = false;
                state_hourly_ch_01_5m_ = true;
                state_hourly_ch_01_15m_ = false;
                state_hourly_ch_01_30m_ = false;
                state_hourly_ch_01_45m_ = false;
                state_hourly_ch_02_5m_ = false;
                state_hourly_ch_02_15m_ = false;
                state_hourly_ch_02_30m_ = false;
                state_hourly_ch_02_45m_ = false;
            } else {
                state_hourly_ch_01_off_ = false;
                state_hourly_ch_01_5m_ = false;
                state_hourly_ch_01_15m_ = false;
                state_hourly_ch_01_30m_ = false;
                state_hourly_ch_01_45m_ = false;
                state_hourly_ch_02_5m_ =  true;
                state_hourly_ch_02_15m_ = false;
                state_hourly_ch_02_30m_ = false;
                state_hourly_ch_02_45m_ = false;
            }
    } else if (msg.find("ventilation_fan_power_profile_hourly_15min") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::ventilation_fan,
            common::power_consumption_profile::low);
            // mutually exclusive mode
            if (state_ventilation_fan_rpm_high_) {
                state_hourly_ch_01_off_ = false;
                state_hourly_ch_01_5m_ = false;
                state_hourly_ch_01_15m_ = true;
                state_hourly_ch_01_30m_ = false;
                state_hourly_ch_01_45m_ = false;
                state_hourly_ch_02_5m_ = false;
                state_hourly_ch_02_15m_ = false;
                state_hourly_ch_02_30m_ = false;
                state_hourly_ch_02_45m_ = false;
            } else {
                state_hourly_ch_01_off_ = false;
                state_hourly_ch_01_5m_ = false;
                state_hourly_ch_01_15m_ = false;
                state_hourly_ch_01_30m_ = false;
                state_hourly_ch_01_45m_ = false;
                state_hourly_ch_02_5m_ =  false;
                state_hourly_ch_02_15m_ = true;
                state_hourly_ch_02_30m_ = false;
                state_hourly_ch_02_45m_ = false;
            }
    } else if (msg.find("ventilation_fan_power_profile_hourly_30min") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::ventilation_fan,
            common::power_consumption_profile::medium);
            if (state_ventilation_fan_rpm_high_) {
                state_hourly_ch_01_off_ = false;
                state_hourly_ch_01_5m_ = false;
                state_hourly_ch_01_15m_ = false;
                state_hourly_ch_01_30m_ = true;
                state_hourly_ch_01_45m_ = false;
                state_hourly_ch_02_5m_ = false;
                state_hourly_ch_02_15m_ = false;
                state_hourly_ch_02_30m_ = false;
                state_hourly_ch_02_45m_ = false;
            } else {
                state_hourly_ch_01_off_ = false;
                state_hourly_ch_01_5m_ = false;
                state_hourly_ch_01_15m_ = false;
                state_hourly_ch_01_30m_ = false;
                state_hourly_ch_01_45m_ = false;
                state_hourly_ch_02_5m_ =  false;
                state_hourly_ch_02_15m_ = false;
                state_hourly_ch_02_30m_ = true;
                state_hourly_ch_02_45m_ = false;
            }
    } else if (msg.find("ventilation_fan_power_profile_hourly_45min") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::ventilation_fan,
            common::power_consumption_profile::high);
            if (state_ventilation_fan_rpm_high_) {
                state_hourly_ch_01_off_ = false;
                state_hourly_ch_01_5m_ = false;
                state_hourly_ch_01_15m_ = false;
                state_hourly_ch_01_30m_ = false;
                state_hourly_ch_01_45m_ = true;
                state_hourly_ch_02_5m_ = false;
                state_hourly_ch_02_15m_ = false;
                state_hourly_ch_02_30m_ = false;
                state_hourly_ch_02_45m_ = false;
            } else {
                state_hourly_ch_01_off_ = false;
                state_hourly_ch_01_5m_ = false;
                state_hourly_ch_01_15m_ = false;
                state_hourly_ch_01_30m_ = false;
                state_hourly_ch_01_45m_ = false;
                state_hourly_ch_02_5m_ =  false;
                state_hourly_ch_02_15m_ = false;
                state_hourly_ch_02_30m_ = false;
                state_hourly_ch_02_45m_ = true;
            }
    } else if (msg.find("ventilation_fan_power_profile_hourly_60min") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::ventilation_fan,
            common::power_consumption_profile::continuous);

        /***** Upper Full Spectrum Light Power Profile *****/
    } else if (msg.find("upper_full_spectrum_light_power_profile_off") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::upper_full_spectrum_light,
            common::power_consumption_profile::off);
            state_daily_ch_01_off_ = true;
            state_daily_ch_01_3h_ = false;
            state_daily_ch_01_6h_ = false;
            state_daily_ch_01_12h_ = false;
            state_daily_ch_01_18h_ = false;
    } else if (msg.find("upper_full_spectrum_light_power_profile_daily_03h") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::upper_full_spectrum_light,
            common::power_consumption_profile::emergency);
            state_daily_ch_01_off_ = false;
            state_daily_ch_01_3h_ = true;
            state_daily_ch_01_6h_ = false;
            state_daily_ch_01_12h_ = false;
            state_daily_ch_01_18h_ = false;
    } else if (msg.find("upper_full_spectrum_light_power_profile_daily_06h") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::upper_full_spectrum_light,
            common::power_consumption_profile::low);
            state_daily_ch_01_off_ = false;
            state_daily_ch_01_3h_ = false;
            state_daily_ch_01_6h_ = true;
            state_daily_ch_01_12h_ = false;
            state_daily_ch_01_18h_ = false;
    } else if (msg.find("upper_full_spectrum_light_power_profile_daily_12h") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::upper_full_spectrum_light,
            common::power_consumption_profile::medium);
            state_daily_ch_01_off_ = false;
            state_daily_ch_01_3h_ = false;
            state_daily_ch_01_6h_ = false;
            state_daily_ch_01_12h_ = true;
            state_daily_ch_01_18h_ = false;
    } else if (msg.find("upper_full_spectrum_light_power_profile_daily_18h") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::upper_full_spectrum_light,
            common::power_consumption_profile::high);
            state_daily_ch_01_off_ = false;
            state_daily_ch_01_3h_ = false;
            state_daily_ch_01_6h_ = false;
            state_daily_ch_01_12h_ = false;
            state_daily_ch_01_18h_ = true;
        
        /***** Lower Full Spectrum Light Power Profile *****/
    } else if (msg.find("lower_full_spectrum_light_power_profile_off") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::lower_full_spectrum_light,
            common::power_consumption_profile::off);
            state_daily_ch_02_off_ = true;
            state_daily_ch_02_3h_ = false;
            state_daily_ch_02_6h_ = false;
            state_daily_ch_02_12h_ = false;
            state_daily_ch_02_18h_ = false;
    } else if (msg.find("lower_full_spectrum_light_power_profile_daily_03h") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::lower_full_spectrum_light,
            common::power_consumption_profile::emergency);
            state_daily_ch_02_off_ = false;
            state_daily_ch_02_3h_ = true;
            state_daily_ch_02_6h_ = false;
            state_daily_ch_02_12h_ = false;
            state_daily_ch_02_18h_ = false;
    } else if (msg.find("lower_full_spectrum_light_power_profile_daily_06h") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::lower_full_spectrum_light,
            common::power_consumption_profile::low);
            state_daily_ch_02_off_ = false;
            state_daily_ch_02_3h_ = false;
            state_daily_ch_02_6h_ = true;
            state_daily_ch_02_12h_ = false;
            state_daily_ch_02_18h_ = false;
    } else if (msg.find("lower_full_spectrum_light_power_profile_daily_12h") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::lower_full_spectrum_light,
            common::power_consumption_profile::medium);
            state_daily_ch_02_off_ = false;
            state_daily_ch_02_3h_ = false;
            state_daily_ch_02_6h_ = false;
            state_daily_ch_02_12h_ = true;
            state_daily_ch_02_18h_ = false;
    } else if (msg.find("lower_full_spectrum_light_power_profile_daily_18h") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::lower_full_spectrum_light,
            common::power_consumption_profile::high);
            state_daily_ch_02_off_ = false;
            state_daily_ch_02_3h_ = false;
            state_daily_ch_02_6h_ = false;
            state_daily_ch_02_12h_ = false;
            state_daily_ch_02_18h_ = true;

        /***** Wind simulator fan Power Profile *****/
    } else if (msg.find("wind_simulation_fan_power_profile_off") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::wind_simulation_fan,
            common::power_consumption_profile::off);
            state_hourly_ch_03_off_ = true;
            state_hourly_ch_03_5m_ = false;
            state_hourly_ch_03_15m_ = false;
            state_hourly_ch_03_30m_ = false;
            state_hourly_ch_03_45m_ = false;
    } else if (msg.find("wind_simulation_fan_power_profile_hourly_05min") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::wind_simulation_fan,
            common::power_consumption_profile::emergency);
            state_hourly_ch_03_off_ = false;
            state_hourly_ch_03_5m_ = true;
            state_hourly_ch_03_15m_ = false;
            state_hourly_ch_03_30m_ = false;
            state_hourly_ch_03_45m_ = false;
    } else if (msg.find("wind_simulation_fan_power_profile_hourly_15min") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::wind_simulation_fan,
            common::power_consumption_profile::low);
            state_hourly_ch_03_off_ = false;
            state_hourly_ch_03_5m_ = false;
            state_hourly_ch_03_15m_ = true;
            state_hourly_ch_03_30m_ = false;
            state_hourly_ch_03_45m_ = false;
    } else if (msg.find("wind_simulation_fan_power_profile_hourly_30min") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::wind_simulation_fan,
            common::power_consumption_profile::medium);
            state_hourly_ch_03_off_ = false;
            state_hourly_ch_03_5m_ = false;
            state_hourly_ch_03_15m_ = false;
            state_hourly_ch_03_30m_ = true;
            state_hourly_ch_03_45m_ = false;
    } else if (msg.find("wind_simulation_fan_power_profile_hourly_45min") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::wind_simulation_fan,
            common::power_consumption_profile::high);
            state_hourly_ch_03_off_ = false;
            state_hourly_ch_03_5m_ = false;
            state_hourly_ch_03_15m_ = false;
            state_hourly_ch_03_30m_ = false;
            state_hourly_ch_03_45m_ = true;
    } else if (msg.find("wind_simulation_fan_power_profile_hourly_60min") !=
               std::string::npos) {
        (*ctx_->user_request_set_power_mode)(
            common::channel_type::wind_simulation_fan,
            common::power_consumption_profile::continuous);
    } else if (msg.find("development_cmd") != std::string::npos) {
        (*ctx_->user_request_development_cmd)(msg);
    } else if (msg.find("channel_states") != std::string::npos) {
        send_channel_states(sock);
    } else if (msg.find("stats") != std::string::npos) {
        send_stats(sock);
    } else {
        send_help_screen(sock);
    }
}

//-------------------------------------------------------------------------------------------------------------------

} // namespace user_interface
} // namespace hydroctrl

//-------------------------------------------------------------------------------------------------------------------
