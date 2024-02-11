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

#include <common/event.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------------

event::event(event_type type) : type_(type) {}

//---------------------------------------------------------------------------------------------------------------------------

event_type event::get_type() { return type_; }

//---------------------------------------------------------------------------------------------------------------------------

socket_event::socket_event(std::shared_ptr<socket> socket)
    : event::event(event_type::socket), socket_(std::move(socket))
{
}

//---------------------------------------------------------------------------------------------------------------------------

std::shared_ptr<socket> socket_event::get_socket() { return socket_; }

//---------------------------------------------------------------------------------------------------------------------------

timer_event::timer_event(uint64_t id) : event::event(event_type::timer), id_(id)
{
}

//---------------------------------------------------------------------------------------------------------------------------

uint64_t timer_event::get_id() { return id_; }

//---------------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl