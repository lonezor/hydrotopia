/*
 *  Hydrotopia UI
 * 
 *  Copyright (C) 2024 Johan Norberg <lonezor@gmail.com>
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

#include <scene/03_settings/control_channel.hpp>

#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <sstream>

void 
control_channel::connect(std::string host, int port)
{
    // Only IPv4 LAN address supported

    struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host.c_str());

    errno = 0;
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
        std::cerr << "socket() failure: " << strerror(errno) << std::endl;
        exit(1); // process respawned in a loop
    }

    errno = 0;
    auto res = ::connect(fd_, (struct sockaddr *) &addr, sizeof(addr));
    if (res < 0) {
        std::cerr << "connect() failure: " << strerror(errno) << std::endl;
        exit(1); // process respawned in a loop
    }

    // Set receive timeout (interactive user interface)
    struct timeval timeout;
    memset(&timeout, 0, sizeof(timeout));
    timeout.tv_sec = 0;
    timeout.tv_usec = 500000;

    errno = 0;
    res = setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    if (res < 0) {
        std::cerr << "SO_RCVTIMEO failure: " << strerror(errno) << std::endl;
        disconnect();
        exit(1); // process respawned in a loop
    }

    // Enable Nagle's algorithm (optional)
    int value = 1;
    res = setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, (char*)&value, sizeof(value));
    if (res < 0) {
        std::cerr << "failed to set TCP_NODELAY" << std::endl;
    }

    std::cout << "Connected to " << host << ":" << port << std::endl;
}

void control_channel::send_cmd(std::string cmd)
{
    buffer_.resize(cmd.size() * 2);
    memset(buffer_.data(), 0, buffer_.size());
    snprintf(buffer_.data(), buffer_.size(), "%s\n", cmd.c_str());

    auto res = send(fd_, buffer_.data(), cmd.size()+1, 0);

    std::cout << "send_cmd res " << res << std::endl;
}

std::string control_channel::query_channel_states()
{
    std::string response;

    send_cmd("channel_states");

    // Do not try forever
    for(int i=0; i < 16; i++) {
        std::cout << i << std::endl;

        buffer_.resize(1024 * 1024);
        memset(buffer_.data(), 0, buffer_.size());

        auto res = recv(fd_, buffer_.data(), buffer_.size() , 0);

        if (res <= 0) {
            send_cmd("channel_states");
            continue;
        }

        for(int idx=0; idx < res; idx++) {
            response += buffer_[idx];
        }

        if (response.find("state_hourly_ch_02_45m: ") != std::string::npos) {
            break;
        }

    }

    return response;
}

void control_channel::update_remote_clock()
{
    auto now = std::chrono::system_clock::now();
    auto time_t_fmt = std::chrono::system_clock::to_time_t(now);
 
    std::stringstream ss;
    ss << "date --set=\"";
    ss << std::put_time(std::localtime(&time_t_fmt), "%a %b %d %H:%M:%S %Y");
    ss << "\"";

    send_cmd(ss.str());
}

void control_channel::disconnect()
{
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;

        std::cout << "Disconnected socket" << std::endl;
    }
}
