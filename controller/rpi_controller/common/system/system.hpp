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

/** @file system.hpp
 *  @brief System wrapper
 */

#include <arpa/inet.h>
#include <array>
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <sstream>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <system_error>
#include <unistd.h>

#include <gsl/gsl>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------------

/** System wrapper */
class system
{
  public:
    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for read function */
    static ssize_t read(int fd, void *buf, size_t nbyte)
    {
        return ::read(fd, buf, nbyte);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for accept4 function */
    template <typename TAccept>
    static int accept4(int fd, TAccept &address, socklen_t *address_len,
                       int flags)
    {
        return ::accept4(fd, reinterpret_cast<struct sockaddr *>(&address),
                         address_len, flags);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for socket function */
    static int socket(int domain, int type, int protocol)
    {
        return ::socket(domain, type, protocol);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for close function */
    static int close(int fd) { return ::close(fd); }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for setsockopt function */
    static int setsockopt(int fd, int level, int option_name,
                          const void *option_value, socklen_t option_len)
    {
        return ::setsockopt(fd, level, option_name, option_value, option_len);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for getsockopt function */
    static int getsockopt(int fd, int level, int option_name,
                          void *option_value, socklen_t *option_len)
    {
        return ::getsockopt(fd, level, option_name, option_value, option_len);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for fcntl function */
    template <typename... ArgsFcntl>
    static int fcntl(int fd, int cmd, ArgsFcntl... args)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg, hicpp-vararg)
        return ::fcntl(fd, cmd, args...);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for bind function */
    template <typename T> static int bind(int fd, const T &addr)
    {
        return ::bind(fd, reinterpret_cast<const struct sockaddr *>(&addr),
                      sizeof(T));
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for listen function */
    static int listen(int fd, int backlog) { return ::listen(fd, backlog); }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for connect function */
    template <typename TConnect>
    static int connect(int fd, TConnect &addr, socklen_t addr_len)
    {
        return ::connect(fd, reinterpret_cast<struct sockaddr *>(&addr),
                         addr_len);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for getpeername function */
    template <typename TGetPeerName>
    static int getpeername(int fd, TGetPeerName &addr, socklen_t *addr_len)
    {
        return ::getpeername(fd, reinterpret_cast<struct sockaddr *>(&addr),
                             addr_len);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for getsockname function */
    template <typename TGetPeerName>
    static int getsockname(int fd, TGetPeerName &addr, socklen_t *addr_len)
    {
        return ::getsockname(fd, reinterpret_cast<struct sockaddr *>(&addr),
                             addr_len);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for getnameinfo function */
    template <typename TAddressToString>
    static std::string address_to_string(const TAddressToString &addr)
    {
        constexpr int arr_len = NI_MAXHOST;
        std::array<char, arr_len> buff{};

        int res =
            ::getnameinfo(reinterpret_cast<const struct sockaddr *>(&addr),
                          sizeof(TAddressToString), buff.data(), buff.size(),
                          nullptr, 0, NI_NUMERICHOST);
        if (res != 0) {
            std::stringstream err_msg;
            err_msg
                << "[system::string_to_address] failed to convert address: res="
                << std::to_string(res);

            // If a system error occurred, the error code can be found in errno.
            if (res == EAI_SYSTEM) {
                throw std::system_error(errno, std::system_category(),
                                        err_msg.str());
            }
            throw std::runtime_error(err_msg.str());
        }
        return std::string(buff.data());
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for getaddrinfo function */
    template <typename TStringToAddress>
    static void string_to_address(const std::string &addr_string,
                                  TStringToAddress &addr)
    {
        struct addrinfo hints
        {};
        struct addrinfo *result = nullptr;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = 0;
        hints.ai_protocol = 0; // Any protocol

        int res = ::getaddrinfo(addr_string.c_str(), "0", &hints, &result);
        if (res != 0 || result == nullptr || result->ai_addr == nullptr ||
            sizeof(addr) < result->ai_addrlen) {
            freeaddrinfo(result);

            std::stringstream err_msg;
            err_msg << "[system::string_to_address] failed to convert '"
                    << addr_string << "'"
                    << ": res=" << std::to_string(res);

            // If a system error occurred, the error code can be found in errno.
            if (res == EAI_SYSTEM) {
                throw std::system_error(errno, std::system_category(),
                                        err_msg.str());
            }
            throw std::runtime_error(err_msg.str());
        }

        // Use first entry
        memcpy(&addr, result->ai_addr, result->ai_addrlen);

        freeaddrinfo(result);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for sendto function */
    template <typename TSendTo, typename USendTo>
    static ssize_t sendto(int fd, gsl::span<TSendTo> buffer, int flags,
                          const USendTo &dst_addr)
    {
        return ::sendto(fd, buffer.data(),
                        static_cast<size_t>(buffer.size_bytes()), flags,
                        reinterpret_cast<const struct sockaddr *>(&dst_addr),
                        sizeof(USendTo));
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for send function */
    template <typename TSend>
    static ssize_t send(int fd, gsl::span<TSend> buffer, int flags)
    {
        return ::send(fd, buffer.data(),
                      static_cast<size_t>(buffer.size_bytes()), flags);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for recv from function */
    template <typename TRecvfrom, typename URecvfrom>
    static ssize_t recvfrom(int fd, gsl::span<TRecvfrom> buffer, int flags,
                            URecvfrom &src_addr, socklen_t *addr_len)
    {
        return ::recvfrom(
            fd, buffer.data(), static_cast<size_t>(buffer.size_bytes()), flags,
            reinterpret_cast<struct sockaddr *>(&src_addr), addr_len);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for sendmmsg function */
    static int sendmmsg(int fd, struct mmsghdr *vmessages, unsigned int vlen,
                        int flags)
    {
        return ::sendmmsg(fd, vmessages, vlen, flags);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for epoll_create1 function */
    static int epoll_create1(int flags) { return ::epoll_create1(flags); }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for epoll_ctl function */
    static int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
    {
        return ::epoll_ctl(epfd, op, fd, event);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for epoll_wait function */
    static int epoll_wait(int epfd, struct epoll_event *events, int maxevents,
                          int timeout)
    {

        return ::epoll_wait(epfd, events, maxevents, timeout);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for timerfd_create function */
    static int timerfd_create(int fd, int flags)
    {
        return ::timerfd_create(fd, flags);
    }

    //---------------------------------------------------------------------------------------------------------------------------

    /** Wrapper method for timerfd_settime function */
    static int timerfd_settime(int fd, int flags,
                               const struct itimerspec *new_value,
                               struct itimerspec *old_value)
    {
        return ::timerfd_settime(fd, flags, new_value, old_value);
    }

    //---------------------------------------------------------------------------------------------------------------------------
};

} // namespace common
} // namespace hydroctrl
