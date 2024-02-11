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

/** @file event.hpp
 * @brief Event data types
 */

#include <common/event.hpp>
#include <common/network/socket.hpp>
#include <memory>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------------

/** Event type */
enum class event_type
{
    /** Timer event */
    timer = 1000,

    /** Socket event */
    socket = 1001,
};

//---------------------------------------------------------------------------------------------------------------------------

/** Event */
class event
{
  public:
    /** @brief Constructor
     *
     * @param type  Event type
     */
    explicit event(event_type type);

    /** @brief Get event type
     *
     * @return event_type
     */
    virtual event_type get_type();

  private:
    /** Event type */
    event_type type_;
};

//---------------------------------------------------------------------------------------------------------------------------

/** Socket event */
class socket_event : public event
{
  public:
    /** @brief Constructor
     *
     * @param socket  Socket
     */
    explicit socket_event(std::shared_ptr<socket> socket);

    /** @brief Get socket
     *
     * @return Socket
     */
    std::shared_ptr<socket> get_socket();

  private:
    /** Socket */
    std::shared_ptr<socket> socket_;
};

//---------------------------------------------------------------------------------------------------------------------------

/** Timer event */
class timer_event : public event
{
  public:
    /** @brief Constructor
     *
     * @param id  Identifier
     */
    explicit timer_event(uint64_t id);

    /** @brief Get identifier
     *
     * @return Identifier
     */
    uint64_t get_id();

  private:
    /** Identifier */
    uint64_t id_;
};

//---------------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl