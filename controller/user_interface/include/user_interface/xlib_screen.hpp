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

#include <string>
#include <vector>
#include <memory>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/X.h>
#include <cairo.h>
#include <cairo-xlib.h>


#include <graphics_context/surface.hpp>
#include <user_interface/screen.hpp>
#include <user_interface/ui_event.hpp>
#include <user_interface/button.hpp>

class xlib_screen : public screen
{
    public:
        xlib_screen(int width, int height, int xpos, int ypos, std::string title, bool fullscreen);

        ~xlib_screen();

        std::vector<std::shared_ptr<ui_event>> poll_events();

        void button_event(button flag, bool pressed);

        void close();

    private:
        int xpos_;

        int ypos_;

        std::string title_;

        bool fullscreen_;

        int screen_{-1};

        Window window_{0};

        Display* display_{nullptr};

        Atom wm_delete_{0};

        bool closed_{false};

        button button_state_{button::none};
};

