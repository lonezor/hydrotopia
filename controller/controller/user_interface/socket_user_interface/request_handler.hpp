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
#include <common/controller_ctx.hpp>
#include <common/event.hpp>
#include <common/io_monitor.hpp>
#include <common/network/socket.hpp>

namespace hydroctrl {
namespace user_interface {

//---------------------------------------------------------------------------------------------------------------------

/** Message state */
struct msg_state
{
    /** Placeholder */
    int abc;
};

//---------------------------------------------------------------------------------------------------------------------

/** Client expiration */
struct client_expiration
{
    /** Timer id */
    uint64_t timer_id{0};

    /** Socket */
    std::shared_ptr<common::socket> sock{nullptr};
};

//---------------------------------------------------------------------------------------------------------------------

class request_handler
{
  public:
    /** @brief Constructor
     *
     * @param config          Configuration
     * @param packet_engine   Packet engine
     */
    request_handler(std::shared_ptr<common::configuration> config,
                    std::shared_ptr<common::controller_ctx> ctx);

    /** @brief Request handler execution loop
     */
    void run();

  private:
    /*********************************************************************************************************
     ************************************ TOP LEVEL CLIENT HANDLING
     *******************************************
     *********************************************************************************************************/

    /** @brief Create listening socket
     *
     * @param port  TCP port
     *
     * @return Socket
     */
    std::shared_ptr<common::socket> create_listening_socket(int port);

    /** @brief Handle client socket event
     *
     * @param socket_event  Socket event
     */
    void handle_client_socket_event(
        const std::shared_ptr<common::socket_event> &socket_event);

    /** @brief Client setup
     *
     * Administative tasks needed when connecting a new client
     *
     * @param sock  Socket
     */
    void client_setup(const std::shared_ptr<common::socket> &sock);

    /** @brief Client teardown: socket as key
     *
     * Administative tasks needed when disconnecting a new client
     *
     * @param sock  Socket
     */
    void client_teardown(const std::shared_ptr<common::socket> &sock);

    /** @brief Client teardown: timer id as key
     *
     * Administative tasks needed for expired sessions
     *
     * @param timer_id   Timer id
     */
    void client_teardown(uint64_t timer_id);

    /** Handle client message */
    void handle_client_msg(const std::shared_ptr<common::socket> &sock,
                           const std::string &msg);

    /** Send help screen */
    void send_help_screen(const std::shared_ptr<common::socket> &sock);

    /** Send channel states */
    void send_channel_states(const std::shared_ptr<common::socket> &sock);

    /** Send stats */
    void send_stats(const std::shared_ptr<common::socket> &sock);

    /** I/O monitor for handling incoming client connections */
    std::shared_ptr<common::io_monitor> io_monitor_;

    /** Server configuration */
    std::shared_ptr<common::configuration> config_;

    /** Controller context */
    std::shared_ptr<common::controller_ctx> ctx_;

    /** Target map (key: socket descriptor) */
    std::unordered_map<int, msg_state> target_map_;

    /** Client expiration map (key: timer id) */
    std::unordered_map<uint64_t, client_expiration> client_expiration_map_;

    /** Expiration counter */
    uint64_t client_expirations_{0};

    bool state_ventilation_fan_rpm_low_{true};
    bool state_ventilation_fan_rpm_high_{false};

    bool state_daily_ch_01_off_{true};
    bool state_daily_ch_01_3h_{false};
    bool state_daily_ch_01_6h_{false};
    bool state_daily_ch_01_12h_{false};
    bool state_daily_ch_01_18h_{false};

    bool state_daily_ch_02_off_{true};
    bool state_daily_ch_02_3h_{false};
    bool state_daily_ch_02_6h_{false};
    bool state_daily_ch_02_12h_{false};
    bool state_daily_ch_02_18h_{false};

    bool state_hourly_ch_01_off_{true};
    bool state_hourly_ch_01_5m_{false};
    bool state_hourly_ch_01_15m_{false};
    bool state_hourly_ch_01_30m_{false};
    bool state_hourly_ch_01_45m_{false};

    bool state_hourly_ch_02_5m_{false};
    bool state_hourly_ch_02_15m_{false};
    bool state_hourly_ch_02_30m_{false};
    bool state_hourly_ch_02_45m_{false};

    bool state_hourly_ch_03_off_{true};
    bool state_hourly_ch_03_5m_{false};
    bool state_hourly_ch_03_15m_{false};
    bool state_hourly_ch_03_30m_{false};
    bool state_hourly_ch_03_45m_{false};
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace user_interface
} // namespace hydroctrl
