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

#include <vector>
#include <memory>

#include <object/object.hpp>
#include <user_interface/ui_event.hpp>
#include <graphics_context/rendering_context.hpp>
#include <graphics_context/surface_cache.hpp>

class scene
{
    public:

        scene(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache);

        void add_object(std::shared_ptr<object> object);

        void invalidate();

        void set_selected_svg_paths(std::vector<std::string> selection) { selected_svg_paths_ = selection; }

        std::vector<std::string> selected_svg_paths() { return selected_svg_paths_; }

        virtual void draw() = 0;

        virtual void draw(ui_event ev) = 0;

        virtual void begin() {};

        void end() { ended_ = true; }

        bool ended() { return ended_; }

        bool idle_screen() { return idle_screen_; }

        void reset() {
            ended_ = false;
            idle_screen_ = false;
        }

    protected:
        std::shared_ptr<rendering_context> ctx_;
        std::shared_ptr<surface_cache> sur_cache_;
        std::vector<std::shared_ptr<object>> objects_;
        std::vector<std::string> selected_svg_paths_;
        bool ended_{false};
        bool idle_screen_{false};
        uint64_t draw_counter_{0};
};


