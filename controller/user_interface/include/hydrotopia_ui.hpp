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

#include <memory>

#include <user_interface/xlib_screen.hpp>
#include <user_interface/ui_event.hpp>
#include <graphics_context/surface_cache.hpp>
#include <graphics_context/rendering_context.hpp>
#include <scene/scene.hpp>

using scene_idx = size_t;

class hydrotopia_ui
{
    public:
        hydrotopia_ui(int screen_width, int screen_height, bool fullscreen);

        void run();

    private:
        double target_fps_;
        double current_fps_;
        bool exit_{false};
        bool osd_{false};
        int screen_width_;
        int screen_height_;
        bool fullscreen_;

        int64_t start_ts_;

        std::shared_ptr<xlib_screen> screen_;

        std::shared_ptr<rendering_context> ctx_;

        std::shared_ptr<surface_cache> sur_cache_;

        bool check_ui_events();
        void draw_scene();
        void draw_scene(ui_event ev);
        void draw_on_screen_display();
        void clear_on_screen_display();
        bool initial_expose_event_{false};
        std::string elapsed_time_str(int64_t elapsed_time);

        void scene_init();
    
        scene_idx scene_idx_{0};
        std::map<scene_idx,std::shared_ptr<scene>> scenes_;


};
