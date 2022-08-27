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

/** @file socket.hpp
 *  @brief Socket Life Cycle
 */

#pragma once

#include <iostream>

#include <common/system/system.hpp>

//-------------------------------------------------------------------------------------------------------------------

namespace hydroctrl {
namespace common {

//-------------------------------------------------------------------------------------------------------------------

/** @brief %Socket
 *
 * A %Socket is an endpoint for sending or receiving data over the network.
 * It manages the entire life cycle of the resource, from creation and
 * ensuring its deletion.
 *
 * Numerous APIs operates on the socket. The socket handle (fd) is used for
 * both configuration and data transfer operations.
 */
class socket
{
  public:
    /** @brief Socket constructor
     *
     * Create socket
     *
     * @param domain    Communications domain, e.g. AF_INET or AF_INET6
     * @param type      Type of socket, e.g. SOCK_DGRAM
     * @param protocol  Protocol to be used with the socket, e.g. IPPROTO_UDP
     */
    socket(int domain, int type, int protocol);

    /** @brief Constructor when kernel creates socket using accept()
     *
     * @param fd        File descriptor
     * @param domain    Communications domain, e.g. AF_INET or AF_INET6
     * @param type      Type of socket, e.g. SOCK_DGRAM
     * @param protocol  Protocol to be used with the socket, e.g. IPPROTO_UDP
     */
    socket(int fd, int domain, int type, int protocol);

    /** @brief Destructor
     *
     * The socket is closed
     */
    ~socket();

    /** @brief Copy constructor
     */
    socket(const socket &other) noexcept = delete;

    /** @brief Move constructor
     */
    socket(socket &&other) noexcept;

    /** @brief Copy assignment operator
     */
    socket &operator=(const socket &other) noexcept = delete;

    /** @brief Move assignment operator
     */
    socket &operator=(socket &&other) noexcept;

    /** @brief Get socket handle
     *
     * @return Socket file descriptor
     */
    int get_fd();

    /** @brief Get socket handle pointer
     *
     * @return Pointer to socket file descriptor
     */
    int *get_fd_ptr();

    /** @brief Get domain
     *
     * @return int  Communication domain
     */
    int get_domain();

    /** @brief Get type
     *
     * @return int  Socket type
     */
    int get_type();

    /** @brief Get protocol
     *
     * @return int  Socket protocol
     */
    int get_protocol();

    /** Socket is active */
    bool is_active();

    /** Close socket */
    void close();

  private:
    /** File descriptor */
    int fd_ = -1;

    /** Communication domain */
    int domain_ = 0;

    /** Socket type */
    int type_ = 0;

    /** Socket protocol */
    int protocol_ = 0;
};

//-------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl

//-------------------------------------------------------------------------------------------------------------------