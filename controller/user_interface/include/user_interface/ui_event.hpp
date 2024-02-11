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

#pragma once

#include <user_interface/button.hpp>

enum class ui_event_type
{
    none,
    expose,
    pointer_motion,
    button_press,
    button_release,
    key_press,
    key_release,
    focus_in,
    focus_out,
    enter,
    leave,
    close,
};

class ui_event
{
public:
   ui_event(ui_event_type type) : type_(type) {}
  
   ui_event(ui_event_type type, int x, int y)
   : type_(type)
   , x_(x)
   , y_(y)
   {}

   ui_event(ui_event_type type, int x, int y, button button_state)
   : type_(type)
   , x_(x)
   , y_(y)
   , button_state_(button_state)
   {}
    
   ui_event(ui_event_type type, char c)
   : type_(type)
   , c_(c)
   {}

   ui_event_type get_type() { return type_;}
   int get_x() { return x_;}
   int get_y() { return y_;}
   char get_c() { return c_;}
   button get_button_state() { return button_state_; }

   void set_x(int x) { x_ = x; }
   
   void set_y(int y) { y_ = y; }

private:
    ui_event_type type_{ui_event_type::none};
    int x_{-1};
    int y_{-1};
    char c_{0};
    button button_state_{button::none};
};

