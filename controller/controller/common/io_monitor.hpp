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

/** @file io_monitor.hpp
 * @brief I/O monitor
 */

#include <chrono>
#include <memory>
#include <unordered_map>
#include <vector>

#include <common/event.hpp>
#include <common/network/socket.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

/** Maximum number of epoll events */
constexpr int epoll_max_events = 200;

//---------------------------------------------------------------------------------------------------------------------

/** I/O monitor */
class io_monitor
{
  public:
    /** Constructor */
    io_monitor();

    /** @brief Register one shot timer
     *
     * @param duration Duration
     *
     * @return Timer identifier
     */
    uint64_t register_one_shot_timer(std::chrono::microseconds duration);

    /** @brief Register service socket
     *
     * This registers a listening socket for monitoring.
     *
     * @param socket  Socket
     */
    void register_service_socket(const std::shared_ptr<socket> &socket);

    /** @brief Register client socket
     *
     * This registers accepted sockets for monitoring.
     *
     * @param socket  Socket
     */
    void register_client_socket(const std::shared_ptr<socket> &socket);

    /** @brief Wait for events
     *
     * This waits for events to process. This method is intended to
     * be invoked in a event processing loop.
     *
     * @return Event vector
     */
    std::vector<std::shared_ptr<event>> wait_for_events();

  private:
    /** Setup epoll */
    void epoll_setup();

    /** @brief Add file descriptor to epoll
     *
     * @param fd          File descriptor
     * @param event_type  Event type
     */
    void epoll_add(int fd, event_type event_type);

    /** Teardown epoll */
    void epoll_teardown();

    /** Setup timerfd */
    void timerfd_setup();

    /** Event handler for timerfd */
    std::shared_ptr<timer_event> timerfd_handle_event(int fd);

    /** Event handler for socketfd */
    std::shared_ptr<socket_event> socketfd_handle_event(int fd);

    /** Epoll file descriptor */
    int epoll_fd_{-1};

    /** Timer id */
    uint64_t timer_id_{0};

    /* Epoll events */
    struct epoll_event epoll_events_[epoll_max_events]
    {};

    /** @brief Epoll file descriptor event classification map
     *
     * Key: File descriptor
     * Value: Event type
     */
    std::unordered_map<int, event_type> epoll_fd_cat_map_;

    /** @brief Timer map
     *
     * Key: Timerfd descriptor
     * Value: Timer identifier
     */
    std::unordered_map<int, uint64_t> timer_map_;

    /** @brief Service socket map
     *
     * Key: Service socket file descriptor
     * Value: Socket
     */
    std::unordered_map<int, std::shared_ptr<socket>> service_socket_map_;

    /** @brief Client socket map
     *
     * Key: Client socket file descriptor
     * Value: Socket
     */
    std::unordered_map<int, std::shared_ptr<socket>> client_socket_map_;
};

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
