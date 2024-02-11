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

#include <cstring>

#include <common/network/socket.hpp>

namespace hydroctrl {
namespace common {

//-------------------------------------------------------------------------------------------------------------------

socket::socket(int domain, int type, int protocol)
    : domain_(domain), type_(type), protocol_(protocol)
{
    // Create socket according to specified requirements
    fd_ = system::socket(domain_, type_, protocol_);
    if (fd_ < 0) {
        std::stringstream err_msg;
        err_msg << "[socket::socket] socket("
                << "domain=" << std::to_string(domain_)
                << ", type=" << std::to_string(type_)
                << ", protocol=" << std::to_string(protocol_) << ") --> '"
                << std::string(strerror(errno)) << "'";
        throw std::system_error(errno, std::system_category(), err_msg.str());
    }

    // Retrieve updated protocol value, when available (since Linux 2.6.32)
#ifdef SO_PROTOCOL
    int prot = 0;
    socklen_t prot_size = sizeof(prot);
    int res =
        system::getsockopt(fd_, SOL_SOCKET, SO_PROTOCOL, &prot, &prot_size);
    if (res < 0) {
        std::stringstream err_msg;
        err_msg << "[socket::socket] getsockopt("
                << "domain=" << std::to_string(domain_)
                << ", type=" << std::to_string(type_)
                << ", protocol=" << std::to_string(protocol_) << ") --> '"
                << std::string(strerror(errno)) << "'";
        throw std::system_error(errno, std::system_category(), err_msg.str());
    }
    protocol_ = prot;
#endif
}

//-------------------------------------------------------------------------------------------------------------------

socket::socket(int fd, int domain, int type, int protocol)
    : fd_(fd), domain_(domain), type_(type), protocol_(protocol)
{
}

//-------------------------------------------------------------------------------------------------------------------

socket::~socket()
{
    // Close socket, but only if handle is still available - std::move()
    // handling
    close();
}

//-------------------------------------------------------------------------------------------------------------------

socket::socket(socket &&other) noexcept
    : fd_(other.fd_), domain_(other.domain_), type_(other.type_),
      protocol_(other.protocol_)
{
    fd_ = other.fd_;
    other.fd_ = -1; // No close() in other's destructor

    domain_ = other.domain_;
    type_ = other.type_;
    protocol_ = other.protocol_;
}

//-------------------------------------------------------------------------------------------------------------------

socket &socket::operator=(socket &&other) noexcept
{
    fd_ = other.fd_;
    other.fd_ = -1; // No close() in other's destructor

    domain_ = other.domain_;
    type_ = other.type_;
    protocol_ = other.protocol_;

    return *this;
}

//-------------------------------------------------------------------------------------------------------------------

int socket::get_fd() { return fd_; }

//-------------------------------------------------------------------------------------------------------------------

int *socket::get_fd_ptr() { return &fd_; }

//-------------------------------------------------------------------------------------------------------------------

int socket::get_domain() { return domain_; }

//-------------------------------------------------------------------------------------------------------------------

int socket::get_type() { return type_; }

//-------------------------------------------------------------------------------------------------------------------

int socket::get_protocol() { return protocol_; }

//-------------------------------------------------------------------------------------------------------------------

bool socket::is_active() { return (fd_ != -1); }

//-------------------------------------------------------------------------------------------------------------------

void socket::close()
{
    if (fd_ != -1) {
        int res = system::close(fd_);
        if (res < 0) {
            // No exception is thrown in a destructor; use stderr
            std::cerr << "[socket::~socket] close() failed --> fd="
                      << std::to_string(fd_) << ": "
                      << std::string(strerror(errno)) << std::endl;
        }
        fd_ = -1;
    }
}

//-------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
