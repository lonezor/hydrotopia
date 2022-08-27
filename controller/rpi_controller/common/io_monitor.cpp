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

#include <cmath>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include <common/io_monitor.hpp>
#include <common/log.hpp>

namespace hydroctrl {
namespace common {

/** Second to microsecond multiplier */
constexpr double sec_to_microsec_multiplier = 1000000;

/** Microsecond to nanosecond multiplier */
constexpr double microsec_to_nanosec_multiplier = 1000;

/** Microsecond to sec divider */
constexpr double microsec_to_sec_divider = 1000000;

//---------------------------------------------------------------------------------------------------------------------------

io_monitor::io_monitor() : timer_id_(0) { epoll_setup(); }

//---------------------------------------------------------------------------------------------------------------------------

uint64_t io_monitor::register_one_shot_timer(std::chrono::microseconds duration)
{
    int timer_fd;
    struct itimerspec ts
    {};
    memset(&ts, 0, sizeof(ts));

    timer_fd = system::timerfd_create(CLOCK_MONOTONIC, 0);
    if (timer_fd < 0) {
        throw std::system_error(
            errno, std::system_category(),
            "[io_monitor::register_one_shot_timer] timerfd_create() failed");
    }

    auto usecs = static_cast<double>(duration.count());
    double secs = floor(usecs / microsec_to_sec_divider);
    usecs -= secs * sec_to_microsec_multiplier;
    double nsecs = usecs * microsec_to_nanosec_multiplier;

    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = static_cast<time_t>(secs);
    ts.it_value.tv_nsec = static_cast<time_t>(nsecs);

    int res = system::timerfd_settime(timer_fd, 0, &ts, nullptr);
    if (res < 0) {
        throw std::system_error(
            errno, std::system_category(),
            "[io_monitor::register_one_shot_timer] timerfd_settime() failed");
    }

    epoll_add(timer_fd, event_type::timer);

    timer_id_++;
    timer_map_[timer_fd] = timer_id_;

    return timer_id_;
}

//---------------------------------------------------------------------------------------------------------------------------

void io_monitor::register_service_socket(const std::shared_ptr<socket> &socket)
{
    int fd = socket->get_fd();

    epoll_add(fd, event_type::socket);

    service_socket_map_[fd] = socket;
}

//---------------------------------------------------------------------------------------------------------------------------

void io_monitor::register_client_socket(const std::shared_ptr<socket> &socket)
{
    int fd = socket->get_fd();

    epoll_add(fd, event_type::socket);

    client_socket_map_[fd] = socket;
}

//---------------------------------------------------------------------------------------------------------------------------

std::shared_ptr<timer_event> io_monitor::timerfd_handle_event(int fd)
{
    constexpr int timerfd_rx_buffer_len = 4096;
    std::vector<uint8_t> buffer;
    buffer.resize(timerfd_rx_buffer_len);

    ssize_t len = system::read(fd, buffer.data(), buffer.size());

    if (len <= 0) {
        throw std::system_error(
            errno, std::system_category(),
            "[io_monitor::timerfd_handle_event] read() failed");
    }

    auto it = timer_map_.find(fd);
    if (it != timer_map_.end()) {
        uint64_t id = timer_map_[fd];
        return std::make_shared<timer_event>(id);
    }

    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------------

std::shared_ptr<socket_event> io_monitor::socketfd_handle_event(int fd)
{
    // Service listening socket
    auto it = service_socket_map_.find(fd);
    if (it != service_socket_map_.end()) {
        std::shared_ptr<socket> socket = service_socket_map_[fd];

        struct sockaddr_in addr
        {};
        memset(&addr, 0, sizeof(addr));
        socklen_t len = sizeof(addr);

        int client_fd =
            system::accept4(socket->get_fd(), addr, &len, SOCK_CLOEXEC);
        if (client_fd != -1) {
            auto client_socket = std::make_shared<common::socket>(
                client_fd, AF_INET, SOCK_STREAM, 0);
            register_client_socket(client_socket);
        }

        return nullptr;
    }

    // Client socket
    it = client_socket_map_.find(fd);
    if (it != client_socket_map_.end()) {
        std::shared_ptr<socket> socket = client_socket_map_[fd];
        return std::make_shared<socket_event>(socket);
    }

    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------------

std::vector<std::shared_ptr<event>> io_monitor::wait_for_events()
{
    std::vector<std::shared_ptr<event>> events;
    constexpr int max_events = 100;
    // Using epoll API data structure -
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays)
    struct epoll_event epoll_events[max_events];

    while (events.empty()) {
        // Using epoll API data structure -
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        memset(epoll_events, 0, sizeof(epoll_events));

        // Using epoll API data structure -
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        int res = epoll_wait(epoll_fd_, epoll_events_, max_events, -1);
        if (res > 0) {
            // Go through epoll events
            for (int i = 0; i < res; i++) {
                // Classify type of event and call handle function
                // Using epoll API data structure -
                // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
                int fd = epoll_events_[i].data.fd;

                auto it = epoll_fd_cat_map_.find(fd);

                if (it != epoll_fd_cat_map_.end()) {
                    event_type event_type = epoll_fd_cat_map_[fd];

                    switch (event_type) {
                    case event_type::timer: {
                        auto timer_event = timerfd_handle_event(fd);
                        events.push_back(
                            static_cast<std::shared_ptr<event>>(timer_event));
                        close(fd);
                        break;
                    }
                    case event_type::socket: {
                        auto socket_event = socketfd_handle_event(fd);
                        if (socket_event != nullptr) {
                            events.push_back(
                                static_cast<std::shared_ptr<event>>(
                                    socket_event));
                        }
                        break;
                    }
                    }
                }
            }
        } else if (res < 0) {
            log(log_level_debug, "[io_monitor::wait_for_events] epoll error");
        } else {
            log(log_level_debug, "[io_monitor::wait_for_events] epoll timeout");
        }
    }

    return events;
}

//---------------------------------------------------------------------------------------------------------------------------

void io_monitor::epoll_setup()
{
    memset(&epoll_events_, 0, sizeof(epoll_events_));

    epoll_fd_ = system::epoll_create1(EPOLL_CLOEXEC);
    if (epoll_fd_ < 0) {
        throw std::system_error(
            errno, std::system_category(),
            "[io_monitor::epoll_setup] epoll_create1() failed");
    }
}

//---------------------------------------------------------------------------------------------------------------------------

void io_monitor::epoll_add(int fd, event_type event_type)
{
    struct epoll_event ev
    {};
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    int res = system::epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
    if (res < 0) {
        throw std::system_error(errno, std::system_category(),
                                "[io_monitor::epoll_add] epoll_ctl() failed");
    }

    epoll_fd_cat_map_[fd] = event_type;
}

//---------------------------------------------------------------------------------------------------------------------------

void io_monitor::epoll_teardown() {}

//---------------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl